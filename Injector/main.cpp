#include "stdafx.h"
#include "injector.h"
#include "injector_factory.h"


#if 0
typedef HMODULE(__stdcall *PFN_LoadLibraryW)(LPCWSTR lpLibFileName);
//this code was used to obtain shellcode
PFN_LoadLibraryW g_pLoadLibraryW = LoadLibraryW;
wchar_t* g_pString = L"kernel32.dll";
DWORD _declspec(noinline) Func()
{
	if (nullptr == g_pLoadLibraryW(g_pString))
	{
		return 0xFFFFFFFF;
	}
	return 0;
}
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	const auto app_name = L"C:\\Windows\\SysWOW64\\notepad.exe";

	InjectorFactory injector_factory(app_name);
	injector_factory.m_injector->doInjection();


	return 0;
}