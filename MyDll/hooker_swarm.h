#pragma once
#include "hooker.h"
#include <vector>
#include "detour.h"

class HookerSwarm
{
	// Methods
public:
	bool initializeHooks();
	bool deinitializeHooks();
private:
	bool initializeHook(_In_ LPCTSTR moduleName, _In_ LPCSTR functionName, _In_ void* pfnHook, _Out_ void* pfnOriginal);

	// Attributes
public:
	std::vector<Hooker> m_hookers;
};