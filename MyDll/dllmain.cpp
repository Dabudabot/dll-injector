// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "hooker_swarm.h"

bool APIENTRY DllMain(HMODULE hModule, const DWORD ulReasonForCall, LPVOID lpReserved)
{
	HookerSwarm hookerSwarm(hModule);
	switch (ulReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(nullptr, L"... is loading", L"MyDll.dll", MB_OK);
		hookerSwarm.initializeHooks();
		break;
	case DLL_PROCESS_DETACH:
		hookerSwarm.deinitializeHooks();
		break;
	default: break;
	}
	return true;
}

