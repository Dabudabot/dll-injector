#pragma once


#define SIC_MARK

#if 0
typedef struct entry_point_context
{
	ULONG_PTR remote_remote_entry_point;
} entry_point_context, *pentry_point_context;
#endif

class Injector
{
	// Constructors
public:

	// Methods
public:

	// Overrides
public:
	ULONG_PTR			findRemoteEntryPoint();
	virtual void		doInjection(_In_ LPCTSTR dllHome) = 0;

	// Attributes
public:
	LPCTSTR					m_lpDllName;
	STARTUPINFO				m_startupInfo;
	PROCESS_INFORMATION		m_processInfo;
	HANDLE					m_hProcess;
};

bool InjectDll(const HANDLE hProcess, const LPCTSTR lpFileName, const PVOID pfnLoadLibrary);
ULONG_PTR GetEnryPoint(REMOTE_ARGS_DEFS);
ULONG_PTR GetRemoteLoadLibraryA(REMOTE_ARGS_DEFS);
bool FindEntryPoint(REMOTE_ARGS_DEFS, const PVOID context);
ULONG_PTR FindEntryPoint2(REMOTE_ARGS_DEFS);