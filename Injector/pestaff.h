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

typedef struct _EXPORT_CONTEXT
{
	char* ModuleName;
	char* FunctionName;
	ULONG_PTR RemoteFunctionAddress;
} EXPORT_CONTEXT, *PEXPORT_CONTEXT;

bool FindExport(REMOTE_ARGS_DEFS, PVOID Context);