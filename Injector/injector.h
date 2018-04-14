#pragma once

#define SIC_MARK

#if 0
typedef struct entry_point_context
{
	ULONG_PTR remote_remote_entry_point;
} entry_point_context, *pentry_point_context;
#endif

bool InjectDll(const HANDLE hProcess, const LPCTSTR lpFileName, const PVOID pfnLoadLibrary);
ULONG_PTR GetEnryPoint(REMOTE_ARGS_DEFS);
ULONG_PTR GetRemoteLoadLibraryA(REMOTE_ARGS_DEFS);
bool FindEntryPoint(REMOTE_ARGS_DEFS, const PVOID context);
ULONG_PTR FindEntryPoint2(REMOTE_ARGS_DEFS);