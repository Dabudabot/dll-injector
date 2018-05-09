// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "detour.h"

/**
 * \brief Entry point of DLL
 * \param hModule module caller
 * \param ulReasonForCall call reason 
 * \param lpReserved 
 * \return true in case of success entry
 */
bool APIENTRY DllMain(HMODULE hModule, const DWORD ulReasonForCall, LPVOID lpReserved)
{
	switch (ulReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		//list functions which need to be hooked
		
		MyD3D11CreateDeviceAndSwapChain::hooker.initHook(
			L"d3d11.dll", 
			"D3D11CreateDeviceAndSwapChain", 
			MyD3D11CreateDeviceAndSwapChain::MyFunction);	//easiest way
		MyD3D11CreateDevice::hooker.initHook(
			L"d3d11.dll",
			"D3D11CreateDevice",
			MyD3D11CreateDevice::MyFunction);				//create device -> query interface device1-5 -> query inteface factory 0 - 2 -> create swapchain (5)
		MyCreateDxgiFactory::hooker.initHook(
			L"dxgi.dll",
			"CreateDXGIFactory",
			MyCreateDxgiFactory::MyFunction);				//device already created -> create factory -> create swapchain (1)
		MyCreateDxgiFactory1::hooker.initHook(
			L"dxgi.dll",
			"CreateDXGIFactory1",
			MyCreateDxgiFactory1::MyFunction);				//device already created -> create factory -> create swapchain (4)
			
		break;
	case DLL_PROCESS_DETACH:
		//unhook hooked functions here
		MyD3D11CreateDeviceAndSwapChain::hooker.unhook();
		MyD3D11CreateDevice::hooker.unhook();
		MyCreateDxgiFactory::hooker.unhook();
		MyCreateDxgiFactory1::hooker.unhook();
		break;
	default: break;
	}
	return true;
}
