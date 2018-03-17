// Injector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "remote.h"
#define SIC_MARK

HMODULE WINAPI MyLoadLibrary(
	_In_ LPCTSTR lpFileName
)
{
	return ::LoadLibrary(lpFileName);
}

BOOL InjectDll(HANDLE hProcess, LPCTSTR lpFileName, PVOID pfnLoadLibrary)
{
	BOOL ret = FALSE;
	PVOID lpFileName_remote = NULL;
	HANDLE hRemoteThread = NULL;

	for (;;)
	{
		//allocate remote storage
		DWORD lpFileName_size = (wcslen(lpFileName) + 1) * sizeof(wchar_t);
		lpFileName_remote = VirtualAllocEx(hProcess, NULL,
			lpFileName_size, MEM_COMMIT, PAGE_READWRITE);
		if (NULL == lpFileName_remote)
		{
			printf("VirtualAllocEx returns NULL \n");
			break;
		}

		//fill remote storage with data
		SIZE_T bytesWritten;
		BOOL res = WriteProcessMemory(hProcess, lpFileName_remote,
			lpFileName, lpFileName_size, &bytesWritten);
		if (FALSE == res)
		{
			printf("WriteProcessMemory failed with %d \n", GetLastError());
			break;
		}

		DWORD tid;
		//in case of problems try MyLoadLibrary if this is actually current process
		hRemoteThread = CreateRemoteThread(hProcess,
			NULL, 0, (LPTHREAD_START_ROUTINE)pfnLoadLibrary, lpFileName_remote,
			0, &tid);
		if (NULL == hRemoteThread)
		{
			printf("CreateRemoteThread failed with %d \n", GetLastError());
			break;
		}

		//wait for MyDll initialization
		WaitForSingleObject(hRemoteThread, INFINITE);
		//FUTURE obtain exit code and check

		ret = TRUE;
		break;
	}

	if (!ret)
	{
		if (lpFileName_remote) SIC_MARK;
		//TODO call VirtualFree(...)
	}

	if (hRemoteThread) CloseHandle(hRemoteThread);
	return ret;
}

#define RVA_TO_VA(ptype, base, offset) \
 ((ptype)(((DWORD_PTR)(base)) + (offset)));

PVOID GetExportTable(HANDLE hProcess, PVOID pRemoteImageBase)
{
	PIMAGE_NT_HEADERS pRemotePeHeader =
		RVA_TO_VA(PIMAGE_NT_HEADERS, pRemoteImageBase,
			REMOTE(IMAGE_DOS_HEADER, pRemoteImageBase)->e_lfanew);

	DWORD Signature = REMOTE(IMAGE_NT_HEADERS, pRemotePeHeader)->Signature;

	BYTE sig0 = ((BYTE*)&Signature)[0];
	BYTE sig1 = ((BYTE*)&Signature)[1];
	BYTE sig2 = ((BYTE*)&Signature)[2];
	BYTE sig3 = ((BYTE*)&Signature)[3];

	printf("PeHeader starts with %c%c%d%d \n",
		sig0, sig1, sig2, sig3);

	return nullptr;
}

void ModuleWorker(HANDLE hProcess, int i, HMODULE module)
{
	printf("module %d at %p \n", i, module);
	GetExportTable(hProcess, module);
}

int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	::CreateProcess(L"C:\\Windows\\system32\\notepad.exe", NULL,
		NULL, NULL, TRUE, 0 /*CREATE_SUSPENDED*/, NULL, NULL, &info, &processInfo);

	Sleep(1000);

	HANDLE hProcess = processInfo.hProcess;
	DWORD cbNeeded, cb = 1 * sizeof(HMODULE);
	HMODULE* phModules;
	for (;;)
	{
		phModules = (HMODULE*)malloc(cb);
		BOOL res = EnumProcessModulesEx(
			hProcess,
			phModules,
			cb,
			&cbNeeded,
			LIST_MODULES_ALL);
		if (res == 0)
		{
			printf("EnumProcessModulesEx returns %d, cbNeeded is %d \n",
				GetLastError(), cbNeeded);
			break;
		}

		if (cb == cbNeeded)
		{
			printf("Success, cbNeeded is %d \n", cbNeeded);
			break;
		}

		free(phModules);
		cb = cbNeeded;
		continue;
	}

	for (int i = 0; i < cb / sizeof(HMODULE); i++)
	{
		ModuleWorker(hProcess, i, phModules[i]);
	}

	return 0;
}