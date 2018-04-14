#pragma once

#pragma comment(lib, "ntdll.lib")

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

extern "C" NTSTATUS WINAPI ZwQueryInformationProcess(
	_In_      HANDLE           processHandle,
	_In_      DWORD            processInformationClass,
	_Out_     PVOID            processInformation,
	_In_      ULONG            processInformationLength,
	_Out_opt_ PULONG           returnLength
);
extern "C" NTSYSCALLAPI NTSTATUS NTAPI ZwSuspendProcess(__in HANDLE processHandle);
extern "C" NTSYSCALLAPI NTSTATUS NTAPI ZwResumeProcess(__in HANDLE processHandle);