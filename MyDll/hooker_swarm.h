#pragma once
#include "hooker.h"

class HookerSwarm
{
	// Methods
public:
	bool initializeHooks();
	bool deinitializeHooks();
private:
	void* initializeHook(_In_ LPCTSTR moduleName, _In_ LPCSTR functionName, _In_ void* pfnHook);

	// Attributes
public:
	std::vector<Hooker> m_hookers;
};
