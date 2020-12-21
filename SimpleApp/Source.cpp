#if 0
#include "Header.h"
#include <tchar.h>

int _tmain()
{
  _tprintf(_T("hello\n"));
  foo();
}
#endif

#include <Windows.h>

typedef HMODULE(__stdcall* pfnLoadLibraryW)(LPCWSTR libName);

pfnLoadLibraryW gLoadLibW = LoadLibraryW;
const wchar_t* gLibName = L"D:\\Sources\\REF20\\InjectorDemo\\DllInjector\\Release\\DllInjection.dll";
DWORD _declspec(noinline) Func()
{
  if (nullptr == gLoadLibW(gLibName))
  {
    return 0xFFFFFFFF;
  }
  return 0;
}

int main()
{
  Func();
}