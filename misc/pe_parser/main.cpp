// main.cpp : Defines the entry point for the console application.
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
//  Local functions
//------------------------------------------------------------------------

/**
* \brief just run instruction
*/
void print_help()
{
	std::cout << "input error" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "\t pe_parser.exe pe" << std::endl;
}

/**
* \brief processing the input
* \param argc number of arvgs
* \param argv input arguments
* \param appName output name of the pe
* \return true if arguments ok, false otherwise
*/
BOOLEAN process_args(int argc, char* argv[], std::string* appName)
{
	if (argc != 2)
	{
		print_help();
		return false;
	}

	*appName = argv[1];

	return true;
}

/**
 * \brief starting point of the program
 * \param argc number of arguments
 * \param argv arguments
 * \return 0 if success
 */
int main(int argc, char* argv[])
{
	std::string appName;
	BOOLEAN result;
	BOOLEAN is64;
	PIMAGE_OPTIONAL_HEADER32 opt32 = nullptr;
	PIMAGE_OPTIONAL_HEADER64 opt64 = nullptr;

	result = process_args(argc, argv, &appName);

	if (!result)
	{
		std::cerr << "Invalid arguments" << std::endl;
		return 1;
	}

	Parser parser(appName);

	std::cout << "File " << appName << " mapped" << std::endl;
	std::cout << "\t file starts with magic " << ((char*)parser.get_base())[0] << ((char*)parser.get_base())[1] << std::endl;
	std::cout << "\t file signature 0x" << std::hex << parser.get_nt_header()->Signature << std::endl;

	is64 = parser.is_64();
	if (is64)
	{
		opt64 = PIMAGE_OPTIONAL_HEADER64(parser.get_optional_header());
	}
	else
	{
		opt32 = PIMAGE_OPTIONAL_HEADER32(parser.get_optional_header());
	}

	std::cout << "\t machine is " << (is64 ? "x64" : "x86") << std::endl;
	std::cout << "\t address of entry point 0x" << std::hex << (is64 ? opt64->AddressOfEntryPoint : opt32->AddressOfEntryPoint) << std::endl;
	std::cout << "\t in total we got " << parser.get_number_of_sections() << " sections: " << std::endl;

	for (int i = 0; i < parser.get_number_of_sections(); i++)
	{
		auto t = parser.get_section_header();
		std::cout << "\t\t section " << i << " is " << (char*)t[i].Name << std::endl;
	}

	auto exports = parser.get_export();

	if (nullptr != exports)
	{
		std::cout << "\t exporting with name " << parser.get_export()->port_name << std::endl;

		for (DWORD i = 0; i < parser.get_export()->number_elements; i++)
		{
			std::cout << "\t\t function " << i << " with name "
				<< parser.get_export()->elements[i].function_name << " -> "
				<< parser.get_export()->elements[i].address << std::endl;
		}
	}

	auto imports = parser.get_imports();

	if (nullptr != imports)
	{
		std::cout << "\t in total we got " << parser.get_number_of_imports() << " imports" << std::endl;

		for (int i = 0; i < parser.get_number_of_imports(); i++)
		{
			std::cout << "\t\t " << i << ". "
				<< parser.get_imports()[i].port_name << " with "
				<< parser.get_imports()[i].number_elements << " functions" << std::endl;

			for (DWORD j = 0; j < parser.get_imports()[i].number_elements; j++)
			{
				std::cout << "\t\t\t function " << j << " hint " <<
					parser.get_imports()[i].elements[j].hint
					<< " with name "
					<< parser.get_imports()[i].elements[j].function_name << std::endl;
			}
		}
	}

	return 0;
}

