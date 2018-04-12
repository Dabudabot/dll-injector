// remote.cpp : works with remote memory
//

#include "stdafx.h"

//do not call this function with  suspended hProcess
//free() return value
HMODULE* GetRemoteModules(const HANDLE hProcess, DWORD* pnModules)
{
	DWORD cb_needed, cb = 1 * sizeof(HMODULE);
	HMODULE* ph_modules = nullptr;
	for (;;)
	{
		ph_modules = static_cast<HMODULE*>(malloc(cb));
		const auto res = EnumProcessModulesEx(
			hProcess,
			ph_modules,
			cb,
			&cb_needed,
			LIST_MODULES_ALL);
		if (res == 0)
		{
			printf("EnumProcessModulesEx returns %d, cbNeeded is %d \n",
				GetLastError(), cb_needed);
			free(ph_modules);
			return nullptr;
		}

		if (cb == cb_needed)
		{
			printf("Success, cbNeeded is %d \n", cb_needed);
			break;
		}

		free(ph_modules);
		cb = cb_needed;
		continue;
	}

	*pnModules = cb / sizeof(HMODULE);
	return ph_modules;
}

//execute custom callback for each worker
void RemoteModuleWorker(const HANDLE hProcess, HMODULE* phModules, const DWORD nModules,
                        const module_callback worker, const PVOID workerContext)
{
	for (int i = 0; i < nModules; i++)
	{
		ULONG_PTR module = (ULONG_PTR)(phModules[i]); \
			printf("module %d at %p \n", i, module);
		if (!worker(hProcess, module, workerContext)) break;
	}
}

PIMAGE_NT_HEADERS GetLocalPeHeader(REMOTE_ARGS_DEFS, bool* pis64)
{
	const auto p_local_dos_header = REMOTE(IMAGE_DOS_HEADER, p_remote_image_base);
	const auto p_remote_pe_header = RVA_TO_REMOTE_VA(PIMAGE_NT_HEADERS, p_local_dos_header->e_lfanew);
	const auto p_local_pe_header = REMOTE(IMAGE_NT_HEADERS, p_remote_pe_header);

#if 0
	//check PE signature
	DWORD Signature = pLocalPeHeader->Signature;
	BYTE sig0 = ((BYTE*)&Signature)[0];
	BYTE sig1 = ((BYTE*)&Signature)[1];
	BYTE sig2 = ((BYTE*)&Signature)[2];
	BYTE sig3 = ((BYTE*)&Signature)[3];
	printf("PeHeader starts with %c%c%d%d \n", sig0, sig1, sig2, sig3);

	BYTE* sig = REMOTE_ARRAY_FIXED(BYTE, pRemotePeHeader, 4);
	printf("PeHeader starts with %c%c%d%d \n", sig[0], sig[1], sig[2], sig[3]);

	DWORD n;
	BYTE* sig42 = REMOTE_ARRAY_ZEROENDED(BYTE, pRemotePeHeader, n);
	printf("PeHeader starts with %s (strlen=%d) \n", sig42, n);
#endif

	switch (p_local_pe_header->FileHeader.Machine)
	{
	case IMAGE_FILE_MACHINE_I386:
		assert(sizeof(IMAGE_OPTIONAL_HEADER32) ==
			p_local_pe_header->FileHeader.SizeOfOptionalHeader);
		{
			*pis64 = false;
			const auto p_local_pe_header2 = REMOTE(IMAGE_NT_HEADERS32, p_remote_pe_header);
			free(p_local_pe_header);
			return PIMAGE_NT_HEADERS(p_local_pe_header2);
		}
	case IMAGE_FILE_MACHINE_AMD64:
		assert(sizeof(IMAGE_OPTIONAL_HEADER64) ==
			p_local_pe_header->FileHeader.SizeOfOptionalHeader);
		{
			*pis64 = true;
			const auto p_local_pe_header2 = REMOTE(IMAGE_NT_HEADERS64, p_remote_pe_header);
			free(p_local_pe_header);
			return PIMAGE_NT_HEADERS(p_local_pe_header2);
		}
	default:
		printf("Unsupported hardware platfrom, Machine is %d",
			p_local_pe_header->FileHeader.Machine);
		free(p_local_pe_header);
		return nullptr;
	}
}

bool FindExport(REMOTE_ARGS_DEFS, const PVOID context)
{
	const auto export_context = pexport_context(context);
	export_context->remote_function_address = 0;
	auto b_found = false; //have I found export in this module?
	auto b_iterate_more = true; //should we iterate next module?

	bool is64;
	const auto p_local_pe_header = GetLocalPeHeader(REMOTE_ARGS_CALL, &is64);
	ULONG_PTR p_remote_image_export_descriptor;

	if (is64)
	{
		const auto p_local_pe_header2 = PIMAGE_NT_HEADERS64(p_local_pe_header);
		p_remote_image_export_descriptor = RVA_TO_REMOTE_VA(
			PIMAGE_EXPORT_DIRECTORY,
			p_local_pe_header2->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	}
	else
	{
		const auto p_local_pe_header2 = PIMAGE_NT_HEADERS32(p_local_pe_header);
		p_remote_image_export_descriptor = RVA_TO_REMOTE_VA(
			PIMAGE_EXPORT_DIRECTORY,
			p_local_pe_header2->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	}
	free(p_local_pe_header);

	const auto p_local_image_export_descriptor =
		REMOTE(IMAGE_EXPORT_DIRECTORY, p_remote_image_export_descriptor);

	char* p_local_dll_name = nullptr;
	DWORD* p_local_names = nullptr;
	DWORD* p_local_addrs = nullptr;

	for (;;)
	{
		if (!p_local_image_export_descriptor) break; //no export table, iterate next module

		const auto p_remote_dll_name = RVA_TO_REMOTE_VA(
			char*,
			p_local_image_export_descriptor->Name);
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

//some artifact about working with imports
#if 0
if (is64)
{
	PIMAGE_NT_HEADERS64 pLocalPeHeader3 = (PIMAGE_NT_HEADERS64)pLocalPeHeader2;

	ULONG_PTR pRemoteImageImportDescriptor = RVA_TO_REMOTE_VA(
		PIMAGE_IMPORT_DESCRIPTOR,
		pLocalPeHeader3->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	DWORD nDescs = 0;
	PIMAGE_IMPORT_DESCRIPTOR pLocalImageImportDescriptor =
		REMOTE_ARRAY_ZEROENDED(IMAGE_IMPORT_DESCRIPTOR, pRemoteImageImportDescriptor, nDescs);

	for (DWORD i = 0; i < nDescs; i++)
	{
		ULONG_PTR pRemoteDllName = RVA_TO_REMOTE_VA(
			char*,
			pLocalImageImportDescriptor[i].Name);

		char* pLocalDllName = REMOTE_ARRAY_ZEROENDED_NOLEN(char, pRemoteDllName);
		printf("Imports from %s \n", pLocalDllName);
	}
}
#endif