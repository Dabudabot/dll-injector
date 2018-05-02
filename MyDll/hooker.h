#pragma once
class Hooker
{
	// Structs
public:
	struct Hook
	{
		LPCTSTR m_moduleName;
		LPCSTR	m_functionName;
		bool	m_isHooked;
		void*	m_pOriginalFunction;
		void*	m_pHookFunction;
		char	m_jmp[6];
		char	m_apiBytes[6];
		void*	m_pApiFunction;
	};

	// Constructors
public:
	Hooker(_In_ LPCTSTR moduleName, _In_ LPCSTR functionName, _In_ void* pHookFunction);
	~Hooker();

	// Methods
public:
	bool initHook();
	bool insertHook();

	bool unhook();
	bool freeHook();

	// Attributes
public:
	Hook	m_hook{};
};

