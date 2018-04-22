
#include "stdafx.h"
#include "injector_x64.h"

Injector64::Injector64(const STARTUPINFO startupInfo, const PROCESS_INFORMATION processInfo) : Injector()
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
	getRemoteImageBase();
	findLocalPeHeader();
	findRemoteEntryPoint();
	loopEntryPoint();
	findRemoteLoadLibrary();
	inject();
	deLoopEntryPoint();
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
		return;
	}

	const auto pLocalPeb = Remote::copyRemoteDataType<PEB>(m_hProcess, ULONG_PTR(pbi.PebBaseAddress));

	printf("\n");
	printf("from PEB: %p and %p\n", pLocalPeb->Reserved3[0], pLocalPeb->Reserved3[1]);

	m_pRemoteImageBase = ULONG_PTR(pLocalPeb->Reserved3[1]);
	free(pLocalPeb);
}

bool Injector64::findLocalPeHeader()
{
	const auto pLocalDosHeader = Remote::copyRemoteDataType<IMAGE_DOS_HEADER>(m_hProcess, m_pRemoteImageBase);
	const auto e_lfanew = pLocalDosHeader->e_lfanew;
	free(pLocalDosHeader);

	const auto pRemotePeHeader = rvaToRemoteVa(m_pRemoteImageBase, e_lfanew);
	m_pLocalPeHeader = Remote::copyRemoteDataType<IMAGE_NT_HEADERS64>(m_hProcess, pRemotePeHeader);
}

bool Injector64::findRemoteEntryPoint()
{
	const auto addressOfEntryPoint = m_pLocalPeHeader->OptionalHeader.AddressOfEntryPoint;
	m_remoteEntryPoint = rvaToRemoteVa(m_pRemoteImageBase, addressOfEntryPoint);
}

bool Injector64::loopEntryPoint()
{
	WORD patchedEntryPoint = 0xFEEB;
	Remote::readRemoteDataType<WORD>(m_hProcess, m_remoteEntryPoint, &m_originalEntryPoint);
	Remote::writeRemoteDataType<WORD>(m_hProcess, m_remoteEntryPoint, &patchedEntryPoint);

	ResumeThread(m_processInfo.hThread); //resumed pathed process
	Sleep(1000);
}

bool Injector64::deLoopEntryPoint()
{
	auto status = ZwSuspendProcess(m_hProcess);
	if (status)
	{
		printf("ZwSuspendProcess failed with %x\n", status);
		return;
	}

	Remote::writeRemoteDataType<WORD>(m_hProcess, m_remoteEntryPoint, &m_originalEntryPoint);
	status = ZwResumeProcess(m_hProcess);
	if (status)
	{
		printf("ZwResumeProcess failed with %x\n", status);
		return;
	}
	Sleep(1000);
}

bool Injector64::findRemoteLoadLibrary()
{
	DWORD n_modules;
	const auto ph_modules = getRemoteModules(m_hProcess, &n_modules);

	remoteModuleWorker(m_hProcess, ph_modules, n_modules, findExport, &m_loadLibraryContext);
	printf("%s!%s is at %llu \n", 
		m_loadLibraryContext.m_moduleName, 
		m_loadLibraryContext.m_functionName,
		m_loadLibraryContext.m_remoteFunctionAddress);
	free(ph_modules);

	return true;
}

bool Injector64::findExport()
{
	const auto p_remoteImageExportDescriptor = rvaToRemoteVa(
		m_pRemoteImageBase, 
		m_pLocalPeHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	const auto p_localImageExportDescriptor = Remote::copyRemoteDataType<IMAGE_EXPORT_DIRECTORY>(m_hProcess, p_remoteImageExportDescriptor);

	char* p_local_dll_name = nullptr;
	DWORD* p_local_names = nullptr;
	DWORD* p_local_addrs = nullptr;

	for (;;)
	{
		if (!p_localImageExportDescriptor) break; //no export table, iterate next module

		const auto p_remoteDllName = rvaToRemoteVa(m_pRemoteImageBase, p_localImageExportDescriptor->Name);
		p_local_dll_name = REMOTE_ARRAY_ZEROENDED_NOLEN(char, p_remote_dll_name);

		printf("dllName is %s \n", p_local_dll_name);

		if (0 != strcmp(export_context->module_name, p_local_dll_name)) break; //not our dll, iterate next module
		b_iterate_more = false; //we've found our dll no need to iterate more modules

		const auto p_remote_names =
			RVA_TO_REMOTE_VA(DWORD*, p_local_image_export_descriptor->AddressOfNames);
		const auto p_remote_addrs =
			RVA_TO_REMOTE_VA(DWORD*, p_local_image_export_descriptor->AddressOfFunctions);

		p_local_names = REMOTE_ARRAY_FIXED(DWORD, p_remote_names, p_local_image_export_descriptor->NumberOfNames);
		p_local_addrs = REMOTE_ARRAY_FIXED(DWORD, p_remote_addrs, p_local_image_export_descriptor->NumberOfFunctions);

		if (p_local_image_export_descriptor->NumberOfNames != p_local_image_export_descriptor->NumberOfFunctions)
		{
			printf("FindExport: ERROR: VERY STRANGE mismatch NumberOfNames vs NumberOfFunctions \n");
			//TODO printf args ...
			break;
		}

		for (auto i = 0; i < p_local_image_export_descriptor->NumberOfNames; i++)
		{
			const auto p_remote_string = RVA_TO_REMOTE_VA(char*, p_local_names[i]);
			const auto p_local_string = REMOTE_ARRAY_ZEROENDED_NOLEN(char, p_remote_string);

			//printf("Function: %s at %p \n", pLocalString, pLocalAddrs[i]);

			if (0 == strcmp(export_context->function_name, p_local_string))
			{
				b_found = true; //stop iterating, we found it
				export_context->remote_function_address = p_remote_image_base + p_local_addrs[i];
				free(p_local_string);
				break;
			}
			free(p_local_string);
		}

		break;
	}

	if ((!b_iterate_more)&(!b_found))
	{
		printf("FindExport: ERROR: VERY STRANGE function was not found \n");
	}

	if (p_local_names) free(p_local_names);
	if (p_local_addrs) free(p_local_addrs);
	if (p_local_image_export_descriptor) free(p_local_image_export_descriptor);
	if (p_local_dll_name) free(p_local_dll_name);
	return b_iterate_more;
}


bool Injector64::inject()
{

}

