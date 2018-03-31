// remote.cpp : works with remote memory
//

#include "stdafx.h"
#include "pestaff.h"

//execute custom callback from worker
void RemoteModuleWorker(HANDLE hProcess, HMODULE* phModules, DWORD nModules, ModuleCallback worker, PVOID workerContext)
{
	for (auto i = 0; i < nModules; i++)
	{
		const auto module = ULONG_PTR(phModules[i]);
		printf("module %d at %llu \n", i, module);
		if (!worker(hProcess, module, workerContext)) break;
	}
}