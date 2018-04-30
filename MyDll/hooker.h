#pragma once
class Hooker
{
	// Structs
public:
	struct HOOK
	{
		bool isHooked;
		void* pFunction;
		void* pHookFunction;
		char jmp[6];
		char apiBytes[6];
		void* pApiFunction;
	};

	// Constructors
public:
	Hooker();
	~Hooker();

	// Methods
public:
	bool initHook();
	bool initByAddr();
	bool insertHook();
	bool unhook();
	bool freeHook();

	// Attributes
public:
	HMODULE m_hModule;
};

