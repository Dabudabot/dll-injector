// pe_parser.cpp : Implementation of parser class
//

/*++

\author Daulet Tumbayev
\brief Example of parsing PE
\env User mode

--*/

//------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------

#include "stdafx.h"
#include "pe_parser.h"

//------------------------------------------------------------------------
//  Functions
//------------------------------------------------------------------------

Parser::Parser(const std::string& name)
{
	_file = CreateFileA(name.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == _file)
	{
		std::cerr << "CreateFile fails with " << GetLastError() << std::endl;
		throw;
	}

	_file_mapping = CreateFileMapping(_file, NULL, SEC_IMAGE | PAGE_READONLY, 0, 0, NULL);

	if (nullptr == _file_mapping)
	{
		std::cerr << "CreateFileMapping fails with " << GetLastError() << std::endl;
		throw;
	}

	_image_base = MapViewOfFile(_file_mapping, FILE_MAP_READ, 0, 0, 0);

	if (nullptr == _image_base)
	{
		std::cerr << "MapViewOfFile fails with " << GetLastError() << std::endl;
		throw;
	}

	_dos_header = nullptr; // we are too lazy to init it here
	_nt_header = nullptr;
	_machine = 0;
	_section_header = nullptr;
	_opt32 = nullptr;
	_opt64 = nullptr;
	_exprt_dir = nullptr;

	_export = nullptr;
	_imports = nullptr;
	_number_imports = 0;
}

Parser::~Parser()
{
	if (nullptr != _imports)
	{
		if (nullptr != _export->elements)
		{
			delete[] _export->elements;
		}
		delete _export;
	}

	if (nullptr != _imports)
	{
		if (nullptr != _imports->elements)
		{
			delete[] _imports->elements;
		}
		delete[] _imports;
	}

	UnmapViewOfFile(_image_base);
	CloseHandle(_file_mapping);
	CloseHandle(_file);
}

PVOID Parser::get_base()
{
	return _image_base;
}

PIMAGE_DOS_HEADER Parser::get_dos_header()
{
	if (nullptr == _dos_header)
	{
		_dos_header = PIMAGE_DOS_HEADER(_image_base);
	}

	return _dos_header;
}

PIMAGE_NT_HEADERS Parser::get_nt_header()
{
	if (nullptr == _nt_header)
	{
		_nt_header = rvaToVa<PIMAGE_NT_HEADERS>(DWORD_PTR(_image_base), DWORD(get_dos_header()->e_lfanew));
	}

	return _nt_header;
}

BOOLEAN Parser::is_64()
{
	BOOL is64 = FALSE;

	if (0 == _machine)
	{
		_machine = get_nt_header()->FileHeader.Machine;
	}

	switch (_machine)
	{
	case IMAGE_FILE_MACHINE_I386:
		assert(sizeof(IMAGE_OPTIONAL_HEADER32) == get_nt_header()->FileHeader.SizeOfOptionalHeader);
		break;
	case IMAGE_FILE_MACHINE_AMD64:
		assert(sizeof(IMAGE_OPTIONAL_HEADER64) == get_nt_header()->FileHeader.SizeOfOptionalHeader);
		is64 = TRUE;
		break;
	default:
		std::cerr << "Oups, unsupported machine " << _machine << std::endl;
		break;
	}

	return is64;
}

PVOID Parser::get_optional_header()
{
	if (is_64())
	{
		if (nullptr == _opt64)
		{
			_opt64 = PIMAGE_OPTIONAL_HEADER64(&(get_nt_header()->OptionalHeader));
			assert(_opt64->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC || 
				_opt64->Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
				_opt64->Magic == IMAGE_ROM_OPTIONAL_HDR_MAGIC);
		}

		return _opt64;
	}
	else
	{
		if (nullptr == _opt32)
		{
			_opt32 = PIMAGE_OPTIONAL_HEADER32(&(get_nt_header()->OptionalHeader));
			assert(_opt32->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC ||
				_opt32->Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
				_opt32->Magic == IMAGE_ROM_OPTIONAL_HDR_MAGIC);
		}

		return _opt32;
	}
}

PIMAGE_SECTION_HEADER Parser::get_section_header()
{
	if (nullptr != _section_header)
	{
		return _section_header;
	}

	BOOLEAN is64 = is_64();
	DWORD pe_header_size = sizeof(DWORD) +
		sizeof(IMAGE_FILE_HEADER) +
		(is64 ? sizeof(IMAGE_OPTIONAL_HEADER64) : sizeof(IMAGE_OPTIONAL_HEADER32));
	DWORD offset = DWORD(get_dos_header()->e_lfanew) + pe_header_size;

	assert(0 == offset % (is64 ? 8 : 4));

	_section_header = rvaToVa<PIMAGE_SECTION_HEADER>(DWORD_PTR(_image_base), offset);

	return _section_header;
}

WORD Parser::get_number_of_sections()
{
	return get_nt_header()->FileHeader.NumberOfSections;
}

PIMAGE_DATA_DIRECTORY Parser::get_data_dir()
{
	if (is_64())
	{
		PIMAGE_OPTIONAL_HEADER64 opt = PIMAGE_OPTIONAL_HEADER64(get_optional_header());
		return opt->DataDirectory;
	}
	else
	{
		PIMAGE_OPTIONAL_HEADER32 opt = PIMAGE_OPTIONAL_HEADER32(get_optional_header());
		return opt->DataDirectory;
	}
}

PIMAGE_EXPORT_DIRECTORY Parser::get_export_dir()
{
	return rvaToVa<PIMAGE_EXPORT_DIRECTORY>(DWORD_PTR(_image_base), get_data_dir()[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
}

Parser::PPORTS Parser::get_export()
{
	if (nullptr != _export)
	{
		return _export;
	}

	
	DWORD export_dir_start = get_data_dir()[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	DWORD export_dir_end = export_dir_start + get_data_dir()[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

	if (export_dir_start == export_dir_end) return nullptr;

	_export = new PORTS;
	ZeroMemory(_export, sizeof(PORTS));
	PIMAGE_EXPORT_DIRECTORY export_dir = get_export_dir();

	assert(export_dir->NumberOfFunctions == export_dir->NumberOfNames);

	_export->port_name = rvaToVa<PCHAR>(DWORD_PTR(_image_base), export_dir->Name);
	_export->number_elements = export_dir->NumberOfFunctions;
	_export->elements = new PORTS_ELEMENT[_export->number_elements];
	ZeroMemory(_export->elements, sizeof(PORTS_ELEMENT) * _export->number_elements);

	LPDWORD pAddrTable = rvaToVa<LPDWORD>(DWORD_PTR(_image_base), export_dir->AddressOfFunctions);
	LPDWORD pNameTable = rvaToVa<LPDWORD>(DWORD_PTR(_image_base), export_dir->AddressOfNames);
	LPWORD pOrdTable = rvaToVa<LPWORD>(DWORD_PTR(_image_base), export_dir->AddressOfNameOrdinals);

	for (UINT i = 0; i < export_dir->NumberOfFunctions; i++)
	{
		DWORD addr = *pAddrTable++;

		if (0 == addr)
		{
			continue;
		}

		for (UINT j = 0; j < export_dir->NumberOfFunctions; j++)
		{
			if (pOrdTable[j] == i)
			{
				if (addr >= export_dir_start && addr < export_dir_end)
				{
					_export->elements[i].address = rvaToVa<DWORD_PTR>(DWORD_PTR(_image_base), addr);
				}
				else
				{
					_export->elements[i].address = addr;
				}
				
				_export->elements[i].function_name = rvaToVa<PCHAR>(DWORD_PTR(_image_base), pNameTable[j]);
			}
		}
	}

	return _export;
}

PIMAGE_IMPORT_DESCRIPTOR Parser::get_import_desc()
{
	return rvaToVa<PIMAGE_IMPORT_DESCRIPTOR>(DWORD_PTR(_image_base), get_data_dir()[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
}

Parser::PPORTS Parser::get_imports()
{

	if (nullptr != _imports)
	{
		return _imports;
	}

	DWORD import_dir_start = get_data_dir()[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD import_dir_end = import_dir_start + get_data_dir()[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

	if (import_dir_start == import_dir_end) return nullptr;

	WORD amount = get_number_of_imports();

	if (amount == 0)
	{
		return nullptr;
	}

	_imports = new PORTS[amount];
	ZeroMemory(_imports, sizeof(PORTS) * amount);
	PIMAGE_IMPORT_DESCRIPTOR importDesc = get_import_desc();

	// http://cs.usu.edu.ru/docs/pe/dirs1.html
	for (DWORD i = 0; i < amount; i++)
	{
		_imports[i].port_name = rvaToVa<PCHAR>(DWORD_PTR(_image_base), importDesc[i].Name);
		
		if (is_64())
		{
			get_import_elements<PIMAGE_THUNK_DATA64>(&_imports[i], &importDesc[i]);
		}
		else
		{
			get_import_elements<PIMAGE_THUNK_DATA32>(&_imports[i], &importDesc[i]);
		}
	}


	return _imports;
}

WORD Parser::get_number_of_imports()
{
	PIMAGE_IMPORT_DESCRIPTOR importDesc = get_import_desc();
	WORD counter = 0;
	
	while (0 != importDesc->Name)
	{
		importDesc++;
		counter++;
	}

	return counter;
}