// Injector.cpp : Defines the entry point for the console application.
//
// Funtions googled:
// LoadLibrary - to load your dll (eg mydll.dll)
// CreateProcess - to create some process (eg notepad.exe)
// CreateThread - to create new thread and run loadLibrary in separate thread

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
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