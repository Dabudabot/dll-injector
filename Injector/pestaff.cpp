// remote.cpp : works with remote memory
//

#include "stdafx.h"
#include "pestaff.h"

//execute custom callback from worker
void RemoteModuleWorker(HANDLE hProcess, HMODULE* phModules, DWORD nModules, ModuleCallback worker, PVOID workerContext)
{
	for (int i = 0; i < nModules; i++)
	{
		auto module = ULONG_PTR(phModules[i]);
		printf("module %d at %p \n", i, module);
		worker(hProcess, module, workerContext);
	}
}