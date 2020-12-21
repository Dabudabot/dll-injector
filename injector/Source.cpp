
#include <Windows.h>
#include <tchar.h>
#include <winternl.h>
#include <psapi.h>
#include "Remote.h"

#define IF_FAIL_GO(status, func, handler) \
          status = func; \
          if (0 != status) goto handler;

DWORD CreateProc(LPCTSTR appName, HANDLE& hProc, HANDLE& hThread)
{
  STARTUPINFO startupInfo = {};
  PROCESS_INFORMATION processInfo = {};

  if (!CreateProcess(appName,
    nullptr, nullptr, nullptr, true,
    CREATE_SUSPENDED, nullptr, nullptr,
    &startupInfo, &processInfo))
  {
    DWORD err = GetLastError();
    _tprintf(_T("Create process failed (%d)\n"), err);
    return err;
  }

  Sleep(1000);

  hProc = processInfo.hProcess;
  hThread = processInfo.hThread;

  return 0;
}

DWORD LoopEntry(HANDLE hProc, HANDLE hThread, WORD& originalEntry, ULONG_PTR& entryAddr)
{
  PROCESS_BASIC_INFORMATION pbi = {};
  ULONG retLen = 0;

  PEB peb = {};

  ULONG_PTR pRemoteBaseAddress = 0;

  IMAGE_DOS_HEADER localDosHeader = {};
  IMAGE_NT_HEADERS32 localNtHeader = {};

  WORD localEntryPointPatched = 0xFEEB;

  NtQueryInformationProcess(
    hProc, 
    ProcessBasicInformation, 
    &pbi, 
    sizeof(PROCESS_BASIC_INFORMATION), 
    &retLen
  );

  ReadRemote<PEB>(hProc, (ULONG_PTR)pbi.PebBaseAddress, peb);

  pRemoteBaseAddress = (ULONG_PTR)peb.Reserved3[1]; // image base address

  ReadRemote<IMAGE_DOS_HEADER>(hProc, pRemoteBaseAddress, localDosHeader);

  ReadRemote<IMAGE_NT_HEADERS32>(hProc, (ULONG_PTR)(pRemoteBaseAddress + localDosHeader.e_lfanew), localNtHeader);
  
  entryAddr = (ULONG_PTR)(pRemoteBaseAddress + localNtHeader.OptionalHeader.AddressOfEntryPoint);

  ReadRemote<WORD>(hProc, entryAddr, originalEntry);
  WriteRemote<WORD>(hProc, entryAddr, localEntryPointPatched);

  ResumeThread(hThread);

  Sleep(1000);

  return 0;
}

DWORD FindLoadLibrary(HANDLE hProc, HANDLE hThread, ULONG_PTR& requiredFunctionAddr)
{
  LPCSTR requiredModuleName = "KERNEL32.dll";
  LPCSTR requiredFunctionName = "LoadLibraryW";

  DWORD needed = 0;
  DWORD size = 0;
  DWORD amount = 0;
  HMODULE* hModules = nullptr;

  EnumProcessModules(hProc, nullptr, 0, &needed);

  size = needed;
  amount = size / sizeof(HMODULE);

  hModules = (HMODULE*)malloc(size);

  EnumProcessModules(hProc, hModules, size, &needed);

  for (DWORD i = 0; i < amount; i++)
  {
    ULONG_PTR moduleBase = (ULONG_PTR)hModules[i];
    IMAGE_DOS_HEADER localDosHeader = {};
    IMAGE_NT_HEADERS32 localNtHeader = {};

    ReadRemote<IMAGE_DOS_HEADER>(hProc, moduleBase, localDosHeader);

    ReadRemote<IMAGE_NT_HEADERS32>(hProc, (ULONG_PTR)(moduleBase + localDosHeader.e_lfanew), localNtHeader);

    IMAGE_DATA_DIRECTORY exportDir = localNtHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    
    if (0 == exportDir.Size) continue;

    IMAGE_EXPORT_DIRECTORY localModuleExport = {};
    ReadRemote<IMAGE_EXPORT_DIRECTORY>(hProc, (ULONG_PTR)(moduleBase + exportDir.VirtualAddress), localModuleExport);

    CHAR moduleName[MAX_PATH];
    DWORD moduleNameLen = 0;
    ReadRemote<CHAR>(hProc, (ULONG_PTR)(moduleBase + localModuleExport.Name), moduleName, moduleNameLen);

    if (strcmp(moduleName, requiredModuleName)) continue;

    DWORD numberOfFunctions = localModuleExport.NumberOfFunctions;

    ULONG_PTR* functionNamesRva = (ULONG_PTR*)malloc(sizeof(ULONG_PTR) * numberOfFunctions);
    ULONG_PTR* functionAddrsRva = (ULONG_PTR*)malloc(sizeof(ULONG_PTR) * numberOfFunctions);

    DWORD numberOfFunctions2 = numberOfFunctions;
    ReadRemote<ULONG_PTR>(hProc, (ULONG_PTR)(moduleBase + localModuleExport.AddressOfNames), functionNamesRva, numberOfFunctions2);
    numberOfFunctions2 = numberOfFunctions;
    ReadRemote<ULONG_PTR>(hProc, (ULONG_PTR)(moduleBase + localModuleExport.AddressOfFunctions), functionAddrsRva, numberOfFunctions2);

    for (DWORD j = 0; j < numberOfFunctions; j++)
    {
      CHAR functionName[MAX_PATH];
      DWORD functionNameLen = 0;
      ReadRemote<CHAR>(hProc, (ULONG_PTR)(moduleBase + functionNamesRva[j]), functionName, functionNameLen);

      if (!strcmp(functionName, requiredFunctionName))
      {
        // todo may be we are mistaken
        requiredFunctionAddr = (ULONG_PTR)(moduleBase + functionAddrsRva[j]);
        break;
      }
    }

    free(functionNamesRva);
    free(functionAddrsRva);

    break;
  }

  free(hModules);

  return 0;
}

DWORD Inject(HANDLE hProc, HANDLE hThread, ULONG_PTR requiredFunctionAddr)
{
  UCHAR shellx86[]
  {
    /*0x00:*/ 0x90,                               //nops
              0x6A, 0x00,                         //push 0
              0x6A, 0x00,                         //push 0
    /*0x04:*/ 0x68, 0x00, 0x00, 0x00, 0x00,       //push  offset string L"kernel32.dll" (05E2100h)  
    /*0x09:*/ 0xFF, 0x15, 0x00, 0x00, 0x00, 0x00, //call  dword ptr [gLoadLibW (05E337Ch)] 
    /*0x0F:*/ 0xF7, 0xD8,                         //neg   eax
    /*0x11:*/ 0x1B, 0xC0,                         //sbb   eax,eax
    /*0x13:*/ 0xF7, 0xD8,                         //neg   eax
    /*0x15:*/ 0x48,                               //dec   eax 
    /*0x16:*/ 0xC3,                               //ret
    /*0x17:*/ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, // alignment
    /*0x20:*/ 0x00, 0x00, 0x00, 0x00,             //pointer to loadlibraryExW
    /*0x24:*/ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, // alignment
    /*0x30:*/ 0x44, 0x00, 0x3a, 0x00, 0x5c, 0x00, 0x53, 0x00,
              0x6f, 0x00, 0x75, 0x00, 0x72, 0x00, 0x63, 0x00,
              0x65, 0x00, 0x73, 0x00, 0x5c, 0x00, 0x52, 0x00,
              0x45, 0x00, 0x46, 0x00, 0x32, 0x00, 0x30, 0x00,
              0x5c, 0x00, 0x49, 0x00, 0x6e, 0x00, 0x6a, 0x00,
              0x65, 0x00, 0x63, 0x00, 0x74, 0x00, 0x6f, 0x00,
              0x72, 0x00, 0x44, 0x00, 0x65, 0x00, 0x6d, 0x00,
              0x6f, 0x00, 0x5c, 0x00, 0x44, 0x00, 0x6c, 0x00,
              0x6c, 0x00, 0x49, 0x00, 0x6e, 0x00, 0x6a, 0x00,
              0x65, 0x00, 0x63, 0x00, 0x74, 0x00, 0x6f, 0x00,
              0x72, 0x00, 0x5c, 0x00, 0x52, 0x00, 0x65, 0x00,
              0x6c, 0x00, 0x65, 0x00, 0x61, 0x00, 0x73, 0x00,
              0x65, 0x00, 0x5c, 0x00, 0x44, 0x00, 0x6c, 0x00,
              0x6c, 0x00, 0x49, 0x00, 0x6e, 0x00, 0x6a, 0x00,
              0x65, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00,
              0x6f, 0x00, 0x6e, 0x00, 0x2e, 0x00, 0x64, 0x00,
              0x6c, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00
              // String: D:\\Sources\\REF20\\InjectorDemo\\DllInjector\\Release\\DllInjection.dll
  };

  // 1. shellx86 + 0x7 = requiredFunctionAddr
  // 2. [shellx86 + 0x1B] = string
  // 3. shellx86 + 0x1 = shellx86 + 0x1B

  //LPCWSTR dllName = L"D:\\Sources\\REF20\\InjectorDemo\\DllInjector\\Release\\DllInjection.dll";
  //DWORD dllNameSize = sizeof(WCHAR) * (wcslen(dllName) + 1);
  //DWORD shellSize = sizeof(shellx86) + dllNameSize;

  // 1. alloc memory in proc
  PVOID pShellRemote = VirtualAllocEx(hProc, nullptr, sizeof(shellx86), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  ULONG_PTR lpShellRemote = (ULONG_PTR)pShellRemote;
  
  ULONG_PTR stringOffset = lpShellRemote + 0x30;
  ULONG_PTR pointerOffset = lpShellRemote + 0x20;

  // filling first bytes with actual addr of loadLibrary
  memcpy(shellx86 + 0x20, &requiredFunctionAddr, sizeof(ULONG_PTR));

  // fixing call arguments to point to loadLibrary
  memcpy(shellx86 + 0x0C, &pointerOffset, sizeof(ULONG_PTR));

  // fixing push arguments
  memcpy(shellx86 + 0x06, &stringOffset, sizeof(ULONG_PTR));


  // 2. write shell
  SIZE_T written = 0;
  auto result = WriteProcessMemory(hProc, pShellRemote, shellx86, sizeof(shellx86), &written);
  auto lastErr = GetLastError();

  // 3. write dll name
  //ULONG_PTR stringOffset = lpShellRemote + sizeof(shellx86);
  //WriteProcessMemory(hProc, (LPVOID)stringOffset, dllName, dllNameSize, &written);

  // 4.1 fix string push
  //ULONG_PTR pushOffset = lpShellRemote + 0x9;
  //DWORD dStringOffset = stringOffset;
  //auto result = WriteProcessMemory(hProc, (LPVOID)pushOffset, (LPCVOID)dStringOffset, sizeof(DWORD), &written);
  //auto lastErr = GetLastError();

  // 4. fix addr loadlibrary in shell
  //ULONG_PTR funcOffset = lpShellRemote + 0xF;
  //result = WriteProcessMemory(hProc, (LPVOID)funcOffset, (LPCVOID)&requiredFunctionAddr, sizeof(DWORD), &written);
  //lastErr = GetLastError();

  // 5. execute shell in thread
  DWORD tid;
  ULONG_PTR start = lpShellRemote;

  HANDLE hRemoteThread = CreateRemoteThread(hProc,
    nullptr,
    0,
    LPTHREAD_START_ROUTINE(start),
    nullptr,
    0,
    &tid);

  WaitForSingleObject(hRemoteThread, INFINITE);

  DWORD exitCode = 0xf;
  GetExitCodeThread(hRemoteThread, &exitCode);

  CloseHandle(hRemoteThread);

  return 0;
}

extern "C" NTSYSCALLAPI NTSTATUS NTAPI NtSuspendProcess(HANDLE ProcessHandle);
extern "C" NTSYSCALLAPI NTSTATUS NTAPI NtResumeProcess(HANDLE ProcessHandle);

DWORD DeloopEntry(HANDLE hProc, HANDLE hThread, WORD originalEntry, ULONG_PTR entryAddr)
{
  NtSuspendProcess(hProc);

  WriteRemote<WORD>(hProc, entryAddr, originalEntry);

  NtResumeProcess(hProc);

  Sleep(1000);

  return 0;
}

int _tmain()
{
  LPCTSTR appName = _T("C:\\Windows\\SysWOW64\\notepad.exe");
  DWORD err = 0;
  HANDLE hProc;
  HANDLE hThread;
  ULONG_PTR loadLibraryAddr = 0;
  WORD originalEntry = 0;
  ULONG_PTR entryAddr = 0;

  // create process suspended
  IF_FAIL_GO(err, CreateProc(appName, hProc, hThread), MAIN_ERROR_HANLE);

  // loop entry point
  IF_FAIL_GO(err, LoopEntry(hProc, hThread, originalEntry, entryAddr), MAIN_ERROR_HANLE);

  // find loadlibrary
  IF_FAIL_GO(err, FindLoadLibrary(hProc, hThread, loadLibraryAddr), MAIN_ERROR_HANLE);

  // inject shellcode
  IF_FAIL_GO(err, Inject(hProc, hThread, loadLibraryAddr), MAIN_ERROR_HANLE);

  // deloop entrypoint
  IF_FAIL_GO(err, DeloopEntry(hProc, hThread, originalEntry, entryAddr), MAIN_ERROR_HANLE);

  return 0;

MAIN_ERROR_HANLE:
  _tprintf(_T("ERROR: %d\n"), err);

  return err;
}