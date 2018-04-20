// main.cpp -	starting point of the program
//				defines of recieves from user input app name and dll home
//				call injector factory to get correct injector and starts the injection
//

#include "stdafx.h"
#include "injector_factory.h"

/**
 * \brief starting point
 * \param argc number of arguments
 * \param argv arguments
 * \return 0 if success
 */
int _tmain(int argc, _TCHAR* argv[])
{
	//const auto app_name = L"C:\\Windows\\SysWOW64\\notepad.exe";
	const auto app_name = L"C:\\Windows\\System32\\notepad.exe";
	const auto dll_home = L"E:\Documents\Visual Studio 2017\Projects\RE-S18";
	
	InjectorFactory injectorFactory(app_name);
	auto injector = injectorFactory.getInjector();

	injector->doInjection(dll_home);

	return 0;
}