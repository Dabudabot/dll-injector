// Injector.cpp : Defines the entry point for the console application.
//
// Funtions googled:
// LoadLibrary - to load your dll (eg mydll.dll)
// CreateProcess - to create some process (eg notepad.exe)
// CreateThread - to create new thread and run loadLibrary in separate thread
// VirtualAlloc - to reserve space
// WriteProcessMemory - analog of memcpy but for remote process

#include "stdafx.h"

/*
 * To inject dll
 * 
 * Parameters:
 * - hProcess - where to inject
 * - dllPath - what to inject
 */
BOOL InjectDll(HANDLE hProcess, LPCTSTR dllPath);

int _tmain(int argc, _TCHAR* argv[])
{
	const auto h_process = GetCurrentProcess();
	InjectDll(h_process, L"MyDll.dll");
	Sleep(9000);

	//---my dll loaded in separate thread---
#if 0
	//thread id will be stored here
	DWORD thread_id;

	const auto dll_path = L"MyDll.dll";
	//size of path in bytes
	const DWORD dll_path_sz = (wcslen(dll_path) + 1) * sizeof(wchar_t);
	//we need to allocate space for it
	const auto p_dll_path = VirtualAlloc(nullptr, 
		dll_path_sz,
		MEM_COMMIT, 
		PAGE_READWRITE);	

	//now we copy data from variable to this allocated space, 
	//we can check by casting p_dll_path to (wchar_t*)
	memcpy(p_dll_path, dll_path, dll_path_sz);

	//we create new thread and this thread do LoadLibrary
	const auto h_thread = CreateThread(nullptr, 0, 
		LPTHREAD_START_ROUTINE(LoadLibrary), 
		p_dll_path, 0, &thread_id);

	//thead need some time to work, lets wait for it
	Sleep(1000);

	//error handling
	if (h_thread == nullptr)
	{
		const auto error_code = GetLastError();
		printf("CurrentThread failed with: %d \n", error_code);
	}
#endif
	//---my dll lead inseparate thread end---

	//---my dll loaded---
#if 0
	LoadLibrary(L"MyDll.dll");
#endif
	//---mydll load end---

	//---here notepad is created---
#if 0
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION process_info;
	CreateProcess(L"C:\\Windows\\system32\\notepad.exe", 
		nullptr, nullptr, nullptr, 
		true, 0, nullptr, 
		nullptr, &info, &process_info);
#endif
	//---notepad creation end---

	return 0;
}

BOOL InjectDll(const HANDLE hProcess, const LPCTSTR dllPath)
{
	boolean return_value = false;

	//thread id will be stored here
	DWORD thread_id;
	SIZE_T bytes_written;

	//size of path in bytes
	const DWORD dll_path_size = (wcslen(dllPath) + 1) * sizeof(wchar_t);
	//we need to allocate space for it
	const auto p_dll_remote = VirtualAllocEx(hProcess, nullptr,
		dll_path_size, MEM_COMMIT, PAGE_READWRITE);

	//if error
	if (nullptr == p_dll_remote)
	{
		printf("VirtualAllocEx failed with code: %d \n", GetLastError());
		goto virtual_alloc_free;
	} 

	//writing our dll to that space - how it was done by memcpy previously
	const auto write_process_memory_result = WriteProcessMemory(hProcess, p_dll_remote, 
		dllPath, dll_path_size, &bytes_written);

	//if error
	if (false == write_process_memory_result)
	{
		printf("WriteProcessMemory failed with code: %d \n", GetLastError());
		goto write_process_memory_free;
	}

	//we create new thread and this thread do LoadLibrary
	const auto h_remote_thread = CreateRemoteThread(hProcess, nullptr, 0,
		LPTHREAD_START_ROUTINE(LoadLibrary),
		p_dll_remote, 0, &thread_id);

	//error handling
	if (nullptr == h_remote_thread)
	{
		printf("CreateRemoteThread failed with: %d \n", GetLastError());
		goto create_remote_thread_free;
	}

	WaitForSingleObject(h_remote_thread, INFINITE);
	//TODO FUTURE obtain exit code and check

	return_value = true;

	//---postmortem---

	CloseHandle(h_remote_thread);

create_remote_thread_free:
	//TODO free

write_process_memory_free:
	VirtualFreeEx(hProcess, nullptr, dll_path_size, MEM_DECOMMIT);

virtual_alloc_free:
	//TODO free

	return return_value;
}