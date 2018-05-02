#include "stdafx.h"
#include "hooker_swarm.h"

bool HookerSwarm::initializeHooks()
{
	initializeHook(L"D3D11.dll", "D3D11CreateDeviceAndSwapChain", MyD3D11CreateDeviceAndSwapChain, pfnD3D11CreateDeviceAndSwapChain);
	return true;
}

bool HookerSwarm::deinitializeHooks()
{
	for (auto h : m_hookers)
	{
		h.unhook();
		h.freeHook();
	}
	return true;
}

bool HookerSwarm::initializeHook(const LPCTSTR moduleName, const LPCSTR functionName, void* pfnHook, void* pfnOriginal)
{
	Hooker hooker(moduleName, functionName, pfnHook, pfnOriginal);
	const auto result = hooker.initHook();	//check that it was passed
	hooker.insertHook();
	m_hookers.push_back(hooker);
	return result;
}
