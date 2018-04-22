#pragma once

class Injector  // NOLINT
{
	// typedefs
public:
	typedef bool (MODULE_CALLBACK)(HANDLE hProcess, ULONG_PTR pRemoteImageBase, PVOID context);
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
	virtual ~Injector() = default;

	// Methods
public:
	template<typename T>
	static T			rvaToVa(_In_ const DWORD_PTR base, _In_ const DWORD offset) { return T(base + offset); }
	template<typename T>
	static ULONG_PTR	rvaToRemoteVa(_In_ const DWORD_PTR base, _In_ const DWORD offset) 
		{ if (offset) return rvaToVa<T>(base, offset); return 0; };

	static HMODULE*		getRemoteModules(_In_ HANDLE hProcess, _Out_ DWORD* pnModules);
	static void			remoteModuleWorker(_In_ HANDLE hProcess, 
											_In_ HMODULE* phModules, 
											_In_ DWORD nModules, 
											_In_ MODULE_CALLBACK worker,
											_In_ PVOID workerContext);

	// Overrides
public:
	virtual bool		doInjection()			= 0;
	virtual bool		findRemoteEntryPoint()	= 0;
	virtual bool		getRemoteImageBase()	= 0;
	virtual bool		loopEntryPoint()		= 0;
	virtual bool		deLoopEntryPoint()		= 0;
	virtual bool		findLocalPeHeader()		= 0;
	virtual bool		findRemoteLoadLibrary() = 0;
	virtual bool		inject()				= 0;
	virtual bool		findExport()			= 0;
	
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