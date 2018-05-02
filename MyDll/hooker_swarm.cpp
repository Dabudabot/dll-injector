#include "stdafx.h"
#include "hooker_swarm.h"
#include "detour.h"

bool HookerSwarm::initializeHooks()
{
	detour::pfnD3D11CreateDeviceAndSwapChain = detour::pfn_D3D11CreateDeviceAndSwapChain(initializeHook(L"D3D11.dll", "D3D11CreateDeviceAndSwapChain", detour::MyD3D11CreateDeviceAndSwapChain));
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

void* HookerSwarm::initializeHook(const LPCTSTR moduleName, const LPCSTR functionName, void* pfnHook)
{
	Hooker hooker(moduleName, functionName, pfnHook);
	const auto result = hooker.initHook();
	hooker.insertHook();
	m_hookers.push_back(hooker);
	return hooker.m_hook.m_pOriginalFunction;
}
