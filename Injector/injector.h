#pragma once

#define SIC_MARK
#pragma comment(lib, "ntdll.lib")

typedef struct entry_point_context
{
	ULONG_PTR remote_remote_entry_point;
} entry_point_context, *pentry_point_context;

typedef struct peb {
	BYTE                          reserved1[2];
	BYTE                          being_debugged;
	BYTE                          reserved2[1];
	PVOID                         reserved3[2];
	//PPEB_LDR_DATA                 Ldr;
	PVOID                         ldr;
	//PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;
	PVOID                         process_parameters;
	BYTE                          reserved4[104];
	PVOID                         reserved5[52];
	//PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
	PVOID                         post_process_init_routine;
	BYTE                          reserved6[128];
	PVOID                         reserved7[1];
	ULONG                         session_id;
} peb, *ppeb;

typedef struct process_basic_information {
	PVOID						reserved1;
	ppeb						peb_base_address;
	PVOID						reserved2[2];
	ULONG_PTR					unique_process_id;
	PVOID						reserved3;
} process_basic_information;

bool InjectDll(const HANDLE hProcess, const LPCTSTR lpFileName, const PVOID pfnLoadLibrary);
ULONG_PTR GetEnryPoint(REMOTE_ARGS_DEFS);
ULONG_PTR GetRemoteLoadLibraryA(REMOTE_ARGS_DEFS);
bool FindEntryPoint(REMOTE_ARGS_DEFS, const PVOID context);
ULONG_PTR FindEntryPoint2(REMOTE_ARGS_DEFS);

extern "C" NTSTATUS WINAPI ZwQueryInformationProcess(
	_In_      HANDLE           processHandle,
	_In_      DWORD            processInformationClass,
	_Out_     PVOID            processInformation,
	_In_      ULONG            processInformationLength,
	_Out_opt_ PULONG           returnLength
);
extern "C" NTSYSCALLAPI NTSTATUS NTAPI ZwSuspendProcess(__in HANDLE processHandle);
extern "C" NTSYSCALLAPI NTSTATUS NTAPI ZwResumeProcess(__in HANDLE processHandle);
