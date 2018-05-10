// main.cpp : program starts here
#include "stdafx.h"
#include "injector_factory.h"

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
	const auto appName = L"C:\\Users\\Daulet\\source\\repos\\TriangleGreen2\\x64\\Release\\TriangleGreen.exe";
	//const auto appName = L"C:\\Users\\Daulet\\source\\repos\\HDRToneMappingCS11.exe";
	//const auto appName = L"C:\\Users\\Daulet\\Documents\\Visual Studio Projects\\DirectXSamples\\HDRToneMappingCS11\\x64\\Debug\\HDRToneMappingCS11.exe";
	//const auto appName = L"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\C++\\Direct3D11\\Bin\\x64\\SubD11.exe";
	
	InjectorFactory injectorFactory(appName);
	auto injector = injectorFactory.getInjector();

	injector->doInjection();

	return 0;
}