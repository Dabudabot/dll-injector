#pragma once

#pragma comment(lib, "ntdll.lib")

// ReSharper disable CppInconsistentNaming because of Native
typedef struct _PEB {
	BYTE                          Reserved1[2];
	BYTE                          BeingDebugged;
	BYTE                          Reserved2[1];
	PVOID                         Reserved3[2];
	//PPEB_LDR_DATA                 Ldr;
	PVOID                         Ldr;
	//PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;
	PVOID                         ProcessParameters;
	BYTE                          Reserved4[104];
	PVOID                         Reserved5[52];
	//PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
	PVOID                         PostProcessInitRoutine;
	BYTE                          Reserved6[128];
	PVOID                         Reserved7[1];
	ULONG                         SessionId;
} PEB, *PPEB;

typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID						Reserved1;
	PPEB						PebBaseAddress;
	PVOID						Reserved2[2];
	ULONG_PTR					UniqueProcessId;
	PVOID						Reserved3;
} PROCESS_BASIC_INFORMATION;

extern "C" NTSTATUS WINAPI ZwQueryInformationProcess(
	_In_      HANDLE			ProcessHandle,
	_In_      DWORD				ProcessInformationClass,
	_Out_     PVOID				ProcessInformation,
	_In_      ULONG				ProcessInformationLength,
	_Out_opt_ PULONG			ReturnLength
);

extern "C" NTSYSCALLAPI NTSTATUS NTAPI ZwSuspendProcess(__in HANDLE processHandle);
extern "C" NTSYSCALLAPI NTSTATUS NTAPI ZwResumeProcess(__in HANDLE processHandle);
// ReSharper restore CppInconsistentNaming because of Native