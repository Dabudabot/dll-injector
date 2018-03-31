#pragma once

#define RVA_TO_VA(ptype, base, offset) \
	((ptype)(((DWORD_PTR)(base)) + (offset)));

typedef bool (ModuleCallback)(HANDLE hProcess, ULONG_PTR pRemoteModuleImageBase, PVOID context);
void RemoteModuleWorker(HANDLE hProcess, HMODULE* phModules, DWORD nModules, ModuleCallback worker, PVOID workerContext);