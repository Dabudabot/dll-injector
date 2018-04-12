// pestaff.h : works with pe files
//

#pragma once

#define RVA_TO_VA(p_type, base, offset) \
 ((p_type)(((DWORD_PTR)(base)) + (offset)))

HMODULE* GetRemoteModules(HANDLE hProcess, DWORD* pnModules);

typedef bool (module_callback)(REMOTE_ARGS_DEFS, PVOID context);
void RemoteModuleWorker(HANDLE hProcess, HMODULE* phModules, DWORD nModules,
	module_callback worker, PVOID workerContext);

PIMAGE_NT_HEADERS GetLocalPeHeader(REMOTE_ARGS_DEFS, bool* pis64);

typedef struct export_context
{
	char* module_name;
	char* function_name;
	ULONG_PTR remote_function_address;
} export_context, *pexport_context;

bool FindExport(REMOTE_ARGS_DEFS, PVOID context);