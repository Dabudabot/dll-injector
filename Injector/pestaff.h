// pestaff.h : works with pe files
//

#pragma once

#define RVA_TO_VA(ptype, base, offset) \
 ((ptype)(((DWORD_PTR)(base)) + (offset)))

HMODULE* GetRemoteModules(HANDLE hProcess, DWORD* pnModules);

typedef bool (ModuleCallback)(REMOTE_ARGS_DEFS, PVOID Context);
void RemoteModuleWorker(HANDLE hProcess, HMODULE* phModules, DWORD nModules,
	ModuleCallback Worker, PVOID WorkerContext);

PIMAGE_NT_HEADERS GetLocalPeHeader(REMOTE_ARGS_DEFS, bool* pis64);