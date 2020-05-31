// pe_parser.h : parsing magic goes here 

#pragma once

#include "stdafx.h"
#include "utils.h"

class Parser
{
public:

	typedef struct _PORTS_ELEMENT
	{
		DWORD_PTR address;
		PCHAR function_name;
		WORD hint;
		BOOLEAN forwarded;
	} PORTS_ELEMENT, * PPORTS_ELEMENT;

	typedef struct _PORTS
	{
		PCHAR port_name;
		DWORD number_elements;
		PPORTS_ELEMENT elements;
	} PORTS, * PPORTS;

private:

	// critical handles that has to be closed
	HANDLE _file;
	HANDLE _file_mapping;
	PVOID _image_base;

	PIMAGE_DOS_HEADER _dos_header;
	PIMAGE_NT_HEADERS _nt_header;
	WORD _machine;
	PIMAGE_SECTION_HEADER _section_header;
	PIMAGE_OPTIONAL_HEADER32 _opt32;
	PIMAGE_OPTIONAL_HEADER64 _opt64;
	PIMAGE_EXPORT_DIRECTORY _exprt_dir;

	PPORTS _export;
	DWORD _number_imports;
	PPORTS _imports;

public:
	/**
	 * \brief ctor to init the parser
	 * \param name pe application image name
	 */
  Parser(const std::string& name);

	/**
	 * \brief dtor to close handles
	 */
	~Parser();

	PVOID get_base();

	PIMAGE_DOS_HEADER get_dos_header();

	PIMAGE_NT_HEADERS get_nt_header();

	BOOLEAN is_64();

	PVOID get_optional_header();

	PIMAGE_SECTION_HEADER get_section_header();

	WORD get_number_of_sections();

	PIMAGE_DATA_DIRECTORY get_data_dir();

	PIMAGE_EXPORT_DIRECTORY get_export_dir();

	PPORTS get_export();

	PIMAGE_IMPORT_DESCRIPTOR get_import_desc();

	PPORTS get_imports();

	WORD get_number_of_imports();

private:

	template <typename T>
	VOID get_import_elements(PPORTS imports, PIMAGE_IMPORT_DESCRIPTOR imageDesc);

};

template<typename T>
inline VOID Parser::get_import_elements(PPORTS imports, PIMAGE_IMPORT_DESCRIPTOR imageDesc)
{
	T pINT = nullptr;
	T pIAT = nullptr;

	time_t time = imageDesc->TimeDateStamp;
	bool bBounded = true;

	if (imageDesc->OriginalFirstThunk != 0) {
		pINT = rvaToVa<T>(DWORD_PTR(_image_base), imageDesc->OriginalFirstThunk);
		pIAT = rvaToVa<T>(DWORD_PTR(_image_base), imageDesc->FirstThunk);
	}
	else {
		pINT = rvaToVa<T>(DWORD_PTR(_image_base), imageDesc->FirstThunk);
		pIAT = nullptr;
		bBounded = false;
	}

	DWORD number_of_elements = 0;

	for (DWORD j = 0; pINT->u1.Ordinal != 0; j++)
	{
		number_of_elements++;
		pINT++;
	}

	pINT -= number_of_elements;

	if (number_of_elements == 0)
	{
		return;
	}

	imports->number_elements = number_of_elements;
	imports->elements = new PORTS_ELEMENT[imports->number_elements];
	ZeroMemory(imports->elements, sizeof(PORTS_ELEMENT) * imports->number_elements);

	for (DWORD j = 0; j < imports->number_elements; j++)
	{
		if (bBounded && pIAT != nullptr)
		{
			imports->elements[j].address = rvaToVa<DWORD_PTR>(DWORD_PTR(_image_base), DWORD(pIAT->u1.Ordinal));
		}

		if (IMAGE_SNAP_BY_ORDINAL(pINT->u1.Ordinal))
		{
			imports->elements[j].hint = WORD(pINT->u1.Ordinal & ~IMAGE_ORDINAL_FLAG);
		}
		else
		{
			PIMAGE_IMPORT_BY_NAME p = rvaToVa<PIMAGE_IMPORT_BY_NAME>(DWORD_PTR(_image_base), DWORD(pINT->u1.Ordinal));
			imports->elements[j].hint = p->Hint;
			imports->elements[j].function_name = p->Name;
		}

		pINT++;
		pIAT++;
	}
}
