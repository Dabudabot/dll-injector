#pragma once
class Injector  // NOLINT
{
	// typedefs
public:

	// structs
public:
	struct FunctionContext
	{
		char* m_moduleName;
		char* m_functionName;
		ULONG_PTR m_remoteFunctionAddress;
	};

	// Constructors
public:
	Injector(STARTUPINFO startupInfo, PROCESS_INFORMATION processInfo);
	virtual ~Injector() = default;

	// Template functions
public:
	template<typename Type>		static Type			rvaToVa(_In_ DWORD_PTR base, _In_ DWORD offset);
	template<typename Type>		static ULONG_PTR	rvaToRemoteVa(_In_ DWORD_PTR base, _In_ DWORD offset);
	template<typename Peb>		static bool			getRemoteImageBase(_In_ HANDLE hProcess, _Out_ ULONG_PTR* pRemoteImageBase);
	template<typename Headers>	static bool			findRemoteEntryPoint(_In_ ULONG_PTR pRemoteImageBase, _Out_ ULONG_PTR* pRemoteEntryPoint);
	template<typename Headers>	static Headers*		findLocalPeHeader(_In_ ULONG_PTR base);

	// Methods
public:
	static HMODULE*						getRemoteModules(_In_ HANDLE hProcess, _Out_ DWORD* pnModules);
	static PROCESS_BASIC_INFORMATION	getProcessBasicInformation(_In_ HANDLE hProcess);
	static bool							loopEntryPoint(_In_ HANDLE hProcess, 
														_In_ HANDLE hThread,
														_In_ ULONG_PTR remoteEntryPoint, 
														_Out_ WORD* originalEntryPointValue);
	static bool							deLoopEntryPoint(_In_ HANDLE hProcess,
														_In_ ULONG_PTR remoteEntryPoint,
														_In_ WORD* originalEntryPointValue);

	// Overrides
public:
	virtual bool					doInjection() = 0;
	virtual PIMAGE_NT_HEADERS		findLocalPeHeader(ULONG_PTR base) = 0;
	virtual bool					findRemoteLoadLibrary() = 0;
	virtual bool					inject() = 0;
	virtual bool					findExport(ULONG_PTR pRemoteImageBase) = 0;
	
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
bool Injector::getRemoteImageBase(HANDLE hProcess, ULONG_PTR* pRemoteImageBase)
{
	const auto pbi = getProcessBasicInformation(m_hProcess);

	if (pbi.PebBaseAddress)
	{
		printf("pbi PebBaseAddress %llu\n", pbi.PebBaseAddress);
		return false;
	}

	const auto pLocalPeb = Remote::copyRemoteDataType<Peb>(m_hProcess, pbi.PebBaseAddress);

	*pRemoteImageBase = ULONG_PTR(pLocalPeb->ImageBaseAddress);
	free(pLocalPeb);

	printf("%s OK\n", "getRemoteImageBase");
	return true;
}

template <typename Headers>
bool Injector::findRemoteEntryPoint(const ULONG_PTR pRemoteImageBase, ULONG_PTR* pRemoteEntryPoint)
{
	const auto pLocalPeHeader = findLocalPeHeader<Headers>(pRemoteImageBase);
	const auto addressOfEntryPoint = pLocalPeHeader->OptionalHeader.AddressOfEntryPoint;
	*pRemoteEntryPoint = rvaToRemoteVa<PVOID>(pRemoteImageBase, addressOfEntryPoint);
	free(pLocalPeHeader);
	printf("%s OK\n", "findRemoteEntryPoint");
	return true;
}

template <typename Headers>
Headers* Injector::findLocalPeHeader(const ULONG_PTR base)
{
	const auto pLocalDosHeader = Remote::copyRemoteDataType<IMAGE_DOS_HEADER>(m_hProcess, base);
	const auto e_lfanew = pLocalDosHeader->e_lfanew;
	free(pLocalDosHeader);

	const auto pRemotePeHeader = rvaToRemoteVa<Headers*>(base, e_lfanew);
	const auto localPeHeader = Remote::copyRemoteDataType<Headers>(m_hProcess, pRemotePeHeader);

	return localPeHeader;
}
