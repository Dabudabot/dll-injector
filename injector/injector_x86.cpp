// injector_x86.cpp : reslization of injection logic for x86 app
#include "stdafx.h"
#include "injector_x86.h"

Injector86::Injector86(const STARTUPINFO startupInfo, const PROCESS_INFORMATION processInfo) : Injector(startupInfo, processInfo)
{
	m_lpDllName = L"C:\\Users\\Daulet\\source\\repos\\RE-S18\\RE-S18\\Debug\\MyDll.dll";	//path to DLL to be injected
}

bool Injector86::doInjection()
{
	printf("%s", "x86 injection begins\n");
	if (!getRemoteImageBase<PEB32>(m_hProcess, &m_pRemoteImageBase)										||  // NOLINT
		!findRemoteEntryPoint<IMAGE_NT_HEADERS32>(m_hProcess, m_pRemoteImageBase, &m_remoteEntryPoint)	||
		!loopEntryPoint(m_hProcess, m_processInfo.hThread, m_remoteEntryPoint, &m_originalEntryPoint)	||
		!findRemoteLoadLibrary<IMAGE_NT_HEADERS32>(m_hProcess, &m_loadLibraryContext)					||
		!inject(m_hProcess, m_lpDllName, m_shellcode, m_szShellcode, m_loadLibraryContext)				||
		!deLoopEntryPoint(m_hProcess, m_remoteEntryPoint, &m_originalEntryPoint))
	{
		printf("%s", "x86 injection failed\n");
		return false;  // NOLINT
	}
	printf("%s", "x86 injection success\n");
	return true;
}