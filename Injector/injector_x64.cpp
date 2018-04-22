#include "stdafx.h"
#include "injector_x64.h"

Injector64::Injector64(const STARTUPINFO startupInfo, const PROCESS_INFORMATION processInfo): Injector(startupInfo, processInfo)
{
	m_lpDllName = L"E:\\Documents\\Visual Studio 2017\\Projects\\RE-S18\\x64\\Debug\\MyDll.dll";
}

bool Injector64::doInjection()
{
	printf("%s", "x64 injection begins\n");
	if (!getRemoteImageBase<PEB64>(m_hProcess, &m_pRemoteImageBase)											||  // NOLINT
		!findRemoteEntryPoint<IMAGE_NT_HEADERS64>(m_pRemoteImageBase, &m_remoteEntryPoint)					||
		!loopEntryPoint(m_hProcess, m_processInfo.hThread, m_remoteEntryPoint, &m_originalEntryPoint)		||
		!findRemoteLoadLibrary()	||
		!inject()					||
		!deLoopEntryPoint(m_hProcess, m_remoteEntryPoint, &m_originalEntryPoint))
	{
		printf("%s", "x64 injection failed\n");
		return false;  // NOLINT
	}
	printf("%s", "x64 injection success\n");
	return true;
}

//the same
bool Injector64::findRemoteLoadLibrary()
{
	DWORD n_modules;
	const auto ph_modules = getRemoteModules(m_hProcess, &n_modules);

	for (auto i = 0; i < n_modules; i++)
	{
		const auto module = ULONG_PTR(ph_modules[i]);
		printf("	module %d at %llu \n", i, module);
		if (!findExport(module)) break;
	}

	printf("%s!%s is at %llu \n", 
		m_loadLibraryContext.m_moduleName, 
		m_loadLibraryContext.m_functionName,
		m_loadLibraryContext.m_remoteFunctionAddress);
	free(ph_modules);

	if (!m_loadLibraryContext.m_remoteFunctionAddress)  // NOLINT
	{
		printf("%s cannot find LoadLibrary\n", "findRemoteLoadLibrary");
		return false;  // NOLINT
	}
	printf("%s OK\n", "findRemoteLoadLibrary");
	return true;
}

//can be templated
bool Injector64::findExport(const ULONG_PTR pRemoteModuleBase)
{
	auto bFound = false; //have I found export in this module?
	auto bIterateMore = true; //should we iterate next module?
	const auto localPeHeader = findLocalPeHeader(pRemoteModuleBase);

	const auto p_remoteImageExportDescriptor = rvaToRemoteVa<PIMAGE_EXPORT_DIRECTORY>(
		pRemoteModuleBase,
		localPeHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	const auto p_localImageExportDescriptor = Remote::copyRemoteDataType<IMAGE_EXPORT_DIRECTORY>(
		m_hProcess, p_remoteImageExportDescriptor);

	char* p_localDllName = nullptr;
	DWORD* p_localNames = nullptr;
	DWORD* p_localAddrs = nullptr;

	for (;;)
	{
		if (!p_localImageExportDescriptor)
		{
			printf("%s","	no export table, iterate next module\n");
			break; //no export table, iterate next module
		} 

		const auto p_remoteDllName = rvaToRemoteVa<char*>(pRemoteModuleBase, p_localImageExportDescriptor->Name);
		p_localDllName = Remote::copyRemoteArrayZeroEnded<char>(m_hProcess, p_remoteDllName, nullptr);

		printf("	dllName is %s \n", p_localDllName);

		if (0 != strcmp(m_loadLibraryContext.m_moduleName, p_localDllName)) //not our dll, iterate next module
		{
			printf("%s", "	not our dll, iterate next module\n");
			break; //not our dll, iterate next module
		}
		bIterateMore = false; //we've found our dll no need to iterate more modules

		const auto p_remoteNames = rvaToRemoteVa<DWORD*>(pRemoteModuleBase, p_localImageExportDescriptor->AddressOfNames);
		const auto p_remoteAddrs = rvaToRemoteVa<DWORD*>(pRemoteModuleBase, p_localImageExportDescriptor->AddressOfFunctions);
		
		p_localNames = Remote::copyRemoteArrayFixedLength<DWORD>(m_hProcess, p_remoteNames, p_localImageExportDescriptor->NumberOfNames);
		p_localAddrs = Remote::copyRemoteArrayFixedLength<DWORD>(m_hProcess, p_remoteAddrs, p_localImageExportDescriptor->NumberOfFunctions);

		if (p_localImageExportDescriptor->NumberOfNames != p_localImageExportDescriptor->NumberOfFunctions)
		{
			printf("FindExport: ERROR: VERY STRANGE mismatch NumberOfNames vs NumberOfFunctions \n");
			break;
		}

		for (auto i = 0; i < p_localImageExportDescriptor->NumberOfNames; i++)
		{
			const auto p_remoteString = rvaToRemoteVa<char*>(pRemoteModuleBase, p_localNames[i]);
			const auto p_localString = Remote::copyRemoteArrayZeroEnded<char>(m_hProcess, p_remoteString, nullptr);

			//printf("Function: %s at %p \n", pLocalString, pLocalAddrs[i]);

			if (0 == strcmp(m_loadLibraryContext.m_functionName, p_localString))
			{
				bFound = true; //stop iterating, we found it
				m_loadLibraryContext.m_remoteFunctionAddress = pRemoteModuleBase + p_localAddrs[i];
				free(p_localString);
				break;
			}
			free(p_localString);
		}

		break;
	}

	if ((!bIterateMore)&(!bFound))
	{
		printf("FindExport: ERROR: VERY STRANGE function was not found \n");
	}

	if (p_localNames) free(p_localNames);
	if (p_localAddrs) free(p_localAddrs);
	if (p_localImageExportDescriptor) free(p_localImageExportDescriptor);
	if (p_localDllName) free(p_localDllName);
	return bIterateMore;
}

//can use inputs
bool Injector64::inject()
{
	auto ret = false;
	PVOID lp_shellcodeRemote = nullptr;
	HANDLE h_remoteThread = nullptr;
	const DWORD lp_dllNameSize = (wcslen(m_lpDllName) + 1) * sizeof(wchar_t);
	const DWORD lp_shellcodeSize = sizeof(m_shellcode) + lp_dllNameSize;

	for (;;)
	{
		//allocate remote storage
		lp_shellcodeRemote = VirtualAllocEx(m_hProcess, nullptr, lp_shellcodeSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (nullptr == lp_shellcodeRemote)
		{
			printf("VirtualAllocEx returns NULL \n");
			break;
		}
		const auto dp_shellcodeRemote = DWORD_PTR(lp_shellcodeRemote);

		//fill remote storage with actual shellcode
		SIZE_T bytesWritten;
		auto res = WriteProcessMemory(m_hProcess,
			lp_shellcodeRemote, 
			m_shellcode, 
			sizeof(m_shellcode), 
			&bytesWritten);

		if (false == res)
		{
			printf("WriteProcessMemory failed with %d \n", GetLastError());
			break;
		}

		//fill remote storage with string
		res = WriteProcessMemory(m_hProcess, 
			rvaToVa<PVOID>(dp_shellcodeRemote, sizeof(m_shellcode)),
			m_lpDllName, 
			lp_dllNameSize, 
			&bytesWritten);

		if (false == res)
		{
			printf("WriteProcessMemory failed with %d \n", GetLastError());
			break;
		}

		//adjust pfnLoadLibrary
		const DWORD patchedPointerRva = 0x00;
		Remote::writeRemoteDataType<ULONG_PTR>(m_hProcess,
			rvaToVa<ULONG_PTR>(dp_shellcodeRemote, patchedPointerRva),
			&m_loadLibraryContext.m_remoteFunctionAddress);

		DWORD tid;
		//in case of problems try MyLoadLibrary if this is actually current process
		h_remoteThread = CreateRemoteThread(m_hProcess,
			nullptr,
			0, 
			LPTHREAD_START_ROUTINE(rvaToVa<ULONG_PTR>(dp_shellcodeRemote, 0x10)),
			lp_shellcodeRemote,
			0, 
			&tid);
		if (nullptr == h_remoteThread)
		{
			printf("CreateRemoteThread failed with %d \n", GetLastError());
			break;
		}

		//wait for MyDll initialization
		WaitForSingleObject(h_remoteThread, INFINITE);

		DWORD exit_code = 0xDEADFACE;
		GetExitCodeThread(h_remoteThread, &exit_code);
		printf("GetExitCodeThread returns %d\n", exit_code);

		ret = true;
		break;
	}

	if (!ret)
	{
		if (lp_shellcodeRemote)
		{
			VirtualFreeEx(m_hProcess, nullptr, lp_shellcodeSize, MEM_DECOMMIT);
		}
	}

	if (h_remoteThread) CloseHandle(h_remoteThread);
	return ret;
}

