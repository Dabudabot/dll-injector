#include "stdafx.h"
#include "injector_x64.h"

Injector64::Injector64(const STARTUPINFO startupInfo, const PROCESS_INFORMATION processInfo)
{
	m_startupInfo = startupInfo;
	m_processInfo = processInfo;
	m_hProcess = processInfo.hProcess;
	m_lpDllName = L"E:\\Documents\\Visual Studio 2017\\Projects\\RE-S18\\x64\\Debug\\MyDll.dll";

	m_loadLibraryContext.m_moduleName = "KERNEL32.dll";
	m_loadLibraryContext.m_functionName = "LoadLibraryW";
	m_loadLibraryContext.m_remoteFunctionAddress = 0;
}

Injector64::~Injector64()  // NOLINT
{
	free(m_pLocalPeHeader);
}

bool Injector64::doInjection()
{
	printf("%s", "x64 injection begins\n");
	if (!getRemoteImageBase()		||  // NOLINT
		!findRemoteEntryPoint()		||
		!loopEntryPoint()			||
		!findRemoteLoadLibrary()	||
		!inject()					||
		!deLoopEntryPoint())
	{
		printf("%s", "x64 injection failed\n");
		return false;  // NOLINT
	}
	printf("%s", "x64 injection success\n");
	return true;
}

bool Injector64::getRemoteImageBase()
{
	PROCESS_BASIC_INFORMATION pbi;
	ZeroMemory(&pbi, sizeof(pbi));
	DWORD retlen = 0;

	const auto status = ZwQueryInformationProcess(
		m_hProcess,
		0,
		&pbi,
		sizeof(pbi),
		&retlen);

	if (status)
	{
		printf("ZwQueryInformationProcess failed with %x\n", status);
		printf("%s failed\n", "getRemoteImageBase");
		return false;
	}

	const auto pLocalPeb = Remote::copyRemoteDataType<PEB>(m_hProcess, ULONG_PTR(pbi.PebBaseAddress));
	
	m_pRemoteImageBase = ULONG_PTR(pLocalPeb->Reserved3[1]);
	free(pLocalPeb);

	printf("%s OK\n", "getRemoteImageBase");
	return true;
}

PIMAGE_NT_HEADERS Injector64::findLocalPeHeader(const ULONG_PTR base)
{
	const auto pLocalDosHeader = Remote::copyRemoteDataType<IMAGE_DOS_HEADER>(m_hProcess, base);
	const auto e_lfanew = pLocalDosHeader->e_lfanew;
	free(pLocalDosHeader);

	const auto pRemotePeHeader = rvaToRemoteVa<PIMAGE_NT_HEADERS>(base, e_lfanew);
	const auto localPeHeader = Remote::copyRemoteDataType<IMAGE_NT_HEADERS64>(m_hProcess, pRemotePeHeader);

	return localPeHeader;
}

bool Injector64::findRemoteEntryPoint()
{
	m_pLocalPeHeader = PIMAGE_NT_HEADERS64(findLocalPeHeader(m_pRemoteImageBase));
	const auto addressOfEntryPoint = m_pLocalPeHeader->OptionalHeader.AddressOfEntryPoint;
	m_remoteEntryPoint = rvaToRemoteVa<PVOID>(m_pRemoteImageBase, addressOfEntryPoint);

	printf("%s OK\n", "findRemoteEntryPoint");
	return true;
}

bool Injector64::loopEntryPoint()
{
	WORD patchedEntryPoint = 0xFEEB;
	Remote::readRemoteDataType<WORD>(m_hProcess, m_remoteEntryPoint, &m_originalEntryPoint);
	Remote::writeRemoteDataType<WORD>(m_hProcess, m_remoteEntryPoint, &patchedEntryPoint);

	ResumeThread(m_processInfo.hThread); //resumed pathed process
	Sleep(1000);

	printf("%s OK\n", "loopEntryPoint");
	return true;
}

bool Injector64::deLoopEntryPoint()
{
	auto status = ZwSuspendProcess(m_hProcess);
	if (status)
	{
		printf("ZwSuspendProcess failed with %x\n", status);
		printf("%s failed\n", "deLoopEntryPoint");
		return false;
	}

	Remote::writeRemoteDataType<WORD>(m_hProcess, m_remoteEntryPoint, &m_originalEntryPoint);
	status = ZwResumeProcess(m_hProcess);
	if (status)
	{
		printf("ZwResumeProcess failed with %x\n", status);
		printf("%s failed\n", "deLoopEntryPoint");
		return false;
	}
	Sleep(1000);

	printf("%s OK\n", "deLoopEntryPoint");
	return true;
}

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

bool Injector64::findExport(const ULONG_PTR pRemoteModuleBase)
{
	auto bFound = false; //have I found export in this module?
	auto bIterateMore = true; //should we iterate next module?
	const auto localPeHeader = PIMAGE_NT_HEADERS64(findLocalPeHeader(pRemoteModuleBase));

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

