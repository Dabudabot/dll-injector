// main.cpp : program starts here

/*++

\author Daulet Tumbayev
\brief Entry point of the injector app
\env User mode

--*/

//------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------

#include "stdafx.h"
#include "injector_factory.h"

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
	std::cout << "\t injector.exe appname dllname" << std::endl;
}

/**
* \brief processing the input
* \param argc number of arvgs
* \param argv input arguments
* \param appName output name of the application to inject to
* \param dllName output name of the dll injecting to app
* \return true if arguments ok, false otherwise
*/
bool process_args(int argc, _TCHAR* argv[], LPTSTR* appName, LPTSTR* dllName)
{
	if (!appName) return false;
	if (!dllName) return false;

	if (argc != 3)
	{
		print_help();
		return false;
	}

	*appName = argv[1];
	*dllName = argv[2];

	return true;
}

/**
 * \brief starting point of the program
 * defines of recieves from user input app name and dll home
 * call injector factory to get correct injector and starts the injection
 * \param argc number of arguments
 * \param argv arguments
 * \return 0 if success
 */
int _tmain(int argc, _TCHAR* argv[])
{
	LPTSTR appName;
	LPTSTR dllName;
	bool result;
	Injector injector;

	result = process_args(argc, argv, &appName, &dllName);

	if (!result)
	{
		std::cerr << "Invalid arguments" << std::endl;
		return 1;
	}

	result = injector->doInjection(appName, dllName);

	if (!result)
	{
		std::cerr << "Injection failed" << std::endl;
		return 2;
	}

	std::cout << "Injection success" << std::endl;
	return 0;
}