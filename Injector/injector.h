// injector.h : base injector 

#pragma once
class Injector  // NOLINT
{
	// structs
public:
	/**
	 * \brief struct to hold function information adress and name
	 */
	struct FunctionContext
	{
		char* m_moduleName;
		char* m_functionName;
		ULONG_PTR m_remoteFunctionAddress;
	};

	// Constructors
public:
	/**
	 * \brief ctor to init the injection
	 * \param startupInfo default startup info
	 * \param processInfo process to inject in
	 */
	Injector(STARTUPINFO startupInfo, PROCESS_INFORMATION processInfo);
	/**
	 * \brief default dtor
	 */
	virtual ~Injector() = default;

	// Template functions
public:
	/**
	 * \brief remote value to value
	 * \tparam Type to cast in
	 * \param base start addr
	 * \param offset add to base this value
	 * \return casted to Type value
	 */
	template<typename Type>		static Type			rvaToVa(_In_ DWORD_PTR base, _In_ DWORD offset);
	/**
	 * \brief remote value to remote value
	 * \tparam Type to cast in
	 * \param base start addr
	 * \param offset add to base this value
	 * \return pointer to addr
	 */
	template<typename Type>		static ULONG_PTR	rvaToRemoteVa(_In_ DWORD_PTR base, _In_ DWORD offset);
	/**
	 * \brief get image base from remote process
	 * \tparam Peb x86 or x64 PEB struct
	 * \param hProcess process from which to get image base
	 * \param pRemoteImageBase achieved image base stored here
	 * \return true in case of success
	 */
	template<typename Peb>		static bool			getRemoteImageBase(_In_ HANDLE hProcess, _Out_ ULONG_PTR* pRemoteImageBase);
	/**
	 * \brief searches for entry point in remote process
	 * \tparam Headers 64 or 86 image header
	 * \param hProcess process where to search
	 * \param pRemoteImageBase image base of the remote process
	 * \param pRemoteEntryPoint achieved entry point stored here
	 * \return true in case of success
	 */
	template<typename Headers>	static bool			findRemoteEntryPoint(_In_ HANDLE hProcess, _In_ ULONG_PTR pRemoteImageBase, _Out_ ULONG_PTR* pRemoteEntryPoint);
	/**
	 * \brief searches for local PE header
	 * \tparam Headers 64 or 86 image header
	 * \param hProcess process where to search
	 * \param base start addr
	 * \return local PE header
	 */
	template<typename Headers>	static Headers*		findLocalPeHeader(_In_ HANDLE hProcess, _In_ ULONG_PTR base);
	/**
	 * \brief searches for loadLibrary
	 * \tparam Headers 64 or 86 image header
	 * \param hProcess process where to search
	 * \param context function context (name, module, addr)
	 * \return true in case of success
	 */
	template<typename Headers>	static bool			findRemoteLoadLibrary(_In_ HANDLE hProcess, _In_ FunctionContext* context);
	/**
	 * \brief searches for exports
	 * \tparam Headers 64 or 86 image header
	 * \param pRemoteModuleBase module base addr
	 * \param hProcess process where to search
	 * \param context function context (name, module, addr)
	 * \return true if need to search more
	 */
	template<typename Headers>	static bool			findExport(_In_ ULONG_PTR pRemoteModuleBase, _In_ HANDLE hProcess, _In_ FunctionContext* context);

	// Methods
public:
	/**
	 * \brief list remote modules
	 * \param hProcess remote process
	 * \param pnModules achieved number modules here
	 * \return modules
	 */
	static HMODULE*						getRemoteModules(_In_ HANDLE hProcess, _Out_ DWORD* pnModules);
	/**
	 * \brief ProcessBasicInformation is a home of PEB struct
	 * \param hProcess remote process
	 * \return ProcessBasicInformation struct
	 */
	static PROCESS_BASIC_INFORMATION	getProcessBasicInformation(_In_ HANDLE hProcess);
	/**
	 * \brief loops entry point of the remote process
	 * \param hProcess remote process
	 * \param hThread remote thread
	 * \param remoteEntryPoint remote entry point which needed to be looped
	 * \param originalEntryPointValue backup original values here
	 * \return true in case of success
	 */
	static bool							loopEntryPoint(_In_ HANDLE hProcess, 
														_In_ HANDLE hThread,
														_In_ ULONG_PTR remoteEntryPoint, 
														_Out_ WORD* originalEntryPointValue);
	/**
	 * \brief restores original values of the entry point
	 * \param hProcess remote process
	 * \param remoteEntryPoint remote entry point which was looped
	 * \param originalEntryPointValue backup of the original values
	 * \return true in case of success
	 */
	static bool							deLoopEntryPoint(_In_ HANDLE hProcess,
														_In_ ULONG_PTR remoteEntryPoint,
														_In_ WORD* originalEntryPointValue);
	/**
	 * \brief injects dll into the remote process by patching it with shellcode
	 * \param hProcess remote process
	 * \param lpDllName dll which is going to be injected
	 * \param shellcode patch
	 * \param szShellcode size of patch
	 * \param context function to be patched
	 * \return true in case of success
	 */
	static bool							inject(_In_ HANDLE hProcess, 
												_In_ LPCTSTR lpDllName,
												_In_ const UCHAR* shellcode,
												_In_ SIZE_T szShellcode,
												_In_ FunctionContext context);
	

	// Overrides
public:
	/**
	 * \brief injection specific algorithm here
	 * \return true in case of success
	 */
	virtual bool					doInjection() = 0;
	
	// Attributes
public:
	LPCTSTR					m_lpDllName;
	STARTUPINFO				m_startupInfo;
	PROCESS_INFORMATION		m_processInfo;
	HANDLE					m_hProcess;

	ULONG_PTR				m_pRemoteImageBase;
	ULONG_PTR				m_remoteEntryPoint;
	WORD					m_originalEntryPoint;

	FunctionContext			m_loadLibraryContext;
};

// Temlate functions implemantations

template <typename Type>
Type Injector::rvaToVa(const DWORD_PTR base, const DWORD offset)
{
	return Type(base + offset);
}

template <typename Type>
ULONG_PTR Injector::rvaToRemoteVa(const DWORD_PTR base, const DWORD offset)
{
	if (offset)
	{
		return ULONG_PTR(rvaToVa<Type>(base, offset));
	}
	return 0;
}

template <typename Peb>
bool Injector::getRemoteImageBase(const HANDLE hProcess, ULONG_PTR* pRemoteImageBase)
{
	const auto pbi = getProcessBasicInformation(hProcess);

	if (!pbi.PebBaseAddress)
	{
		printf("pbi PebBaseAddress %llu\n", pbi.PebBaseAddress);
		return false;
	}

	const auto pLocalPeb = Remote::copyRemoteDataType<Peb>(hProcess, pbi.PebBaseAddress);

	*pRemoteImageBase = ULONG_PTR(pLocalPeb->ImageBaseAddress);
	free(pLocalPeb);

	printf("%s OK\n", "getRemoteImageBase");
	return true;
}

template <typename Headers>
bool Injector::findRemoteEntryPoint(const HANDLE hProcess, const ULONG_PTR pRemoteImageBase, ULONG_PTR* pRemoteEntryPoint)
{
	const auto pLocalPeHeader = findLocalPeHeader<Headers>(hProcess, pRemoteImageBase);
	const auto addressOfEntryPoint = pLocalPeHeader->OptionalHeader.AddressOfEntryPoint;
	*pRemoteEntryPoint = rvaToRemoteVa<PVOID>(pRemoteImageBase, addressOfEntryPoint);
	free(pLocalPeHeader);
	printf("%s OK\n", "findRemoteEntryPoint");
	return true;
}

template <typename Headers>
Headers* Injector::findLocalPeHeader(const HANDLE hProcess, const ULONG_PTR base)
{
	const auto pLocalDosHeader = Remote::copyRemoteDataType<IMAGE_DOS_HEADER>(hProcess, base);
	const auto eLfanew = pLocalDosHeader->e_lfanew;
	free(pLocalDosHeader);

	const auto pRemotePeHeader = rvaToRemoteVa<Headers*>(base, eLfanew);
	const auto localPeHeader = Remote::copyRemoteDataType<Headers>(hProcess, pRemotePeHeader);

	return localPeHeader;
}

template <typename Headers>
bool Injector::findRemoteLoadLibrary(const HANDLE hProcess, FunctionContext* context)
{
	DWORD n_modules;
	const auto ph_modules = getRemoteModules(hProcess, &n_modules);

	for (unsigned int i = 0; i < n_modules; i++)
	{
		const auto module = ULONG_PTR(ph_modules[i]);
		printf("	module %d at %llu \n", i, module);
		if (!findExport<Headers>(module, hProcess, context)) break;
	}

	printf("%s!%s is at %llu \n",
		context->m_moduleName,
		context->m_functionName,
		context->m_remoteFunctionAddress);
	free(ph_modules);

	if (!context->m_remoteFunctionAddress)  // NOLINT
	{
		printf("%s cannot find LoadLibrary\n", "findRemoteLoadLibrary");
		return false;  // NOLINT
	}
	printf("%s OK\n", "findRemoteLoadLibrary");
	return true;
}

template <typename Headers>
bool Injector::findExport(const ULONG_PTR pRemoteModuleBase, HANDLE hProcess, FunctionContext* context)
{
	auto bFound = false; //have I found export in this module?
	auto bIterateMore = true; //should we iterate next module?
	const auto localPeHeader = findLocalPeHeader<Headers>(hProcess, pRemoteModuleBase);

	const auto p_remoteImageExportDescriptor = rvaToRemoteVa<PIMAGE_EXPORT_DIRECTORY>(
		pRemoteModuleBase,
		localPeHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	const auto p_localImageExportDescriptor = Remote::copyRemoteDataType<IMAGE_EXPORT_DIRECTORY>(
		hProcess, p_remoteImageExportDescriptor);

	char* p_localDllName = nullptr;
	DWORD* p_localNames = nullptr;
	DWORD* p_localAddrs = nullptr;

	for (;;)
	{
		if (!p_localImageExportDescriptor)
		{
			printf("%s", "	no export table, iterate next module\n");
			break; //no export table, iterate next module
		}

		const auto p_remoteDllName = rvaToRemoteVa<char*>(pRemoteModuleBase, p_localImageExportDescriptor->Name);
		p_localDllName = Remote::copyRemoteArrayZeroEnded<char>(hProcess, p_remoteDllName, nullptr);

		printf("	dllName is %s \n", p_localDllName);

		if (0 != strcmp(context->m_moduleName, p_localDllName)) //not our dll, iterate next module
		{
			printf("%s", "	not our dll, iterate next module\n");
			break; //not our dll, iterate next module
		}
		bIterateMore = false; //we've found our dll no need to iterate more modules

		const auto p_remoteNames = rvaToRemoteVa<DWORD*>(pRemoteModuleBase, p_localImageExportDescriptor->AddressOfNames);
		const auto p_remoteAddrs = rvaToRemoteVa<DWORD*>(pRemoteModuleBase, p_localImageExportDescriptor->AddressOfFunctions);

		p_localNames = Remote::copyRemoteArrayFixedLength<DWORD>(hProcess, p_remoteNames, p_localImageExportDescriptor->NumberOfNames);
		p_localAddrs = Remote::copyRemoteArrayFixedLength<DWORD>(hProcess, p_remoteAddrs, p_localImageExportDescriptor->NumberOfFunctions);

		if (p_localImageExportDescriptor->NumberOfNames != p_localImageExportDescriptor->NumberOfFunctions)
		{
			printf("FindExport: ERROR: VERY STRANGE mismatch NumberOfNames vs NumberOfFunctions \n");
			break;
		}

		for (unsigned int i = 0; i < p_localImageExportDescriptor->NumberOfNames; i++)
		{
			const auto p_remoteString = rvaToRemoteVa<char*>(pRemoteModuleBase, p_localNames[i]);
			const auto p_localString = Remote::copyRemoteArrayZeroEnded<char>(hProcess, p_remoteString, nullptr);

			//printf("Function: %s at %p \n", pLocalString, pLocalAddrs[i]);

			if (0 == strcmp(context->m_functionName, p_localString))
			{
				bFound = true; //stop iterating, we found it
				context->m_remoteFunctionAddress = pRemoteModuleBase + p_localAddrs[i];
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
