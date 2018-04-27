#pragma once
class Hooker
{
public:
	Hooker();
	~Hooker();

	bool initHook();
	bool initByAddr();
	bool insertHook();
	bool unhook();
	bool freeHook();

	struct HOOK 
	{
		bool isHooked;
		void* pFunction;
		void* pHookFunction;
		char jmp[6];
		char apiBytes[6];
		void* pApiFunction;
	};
};

