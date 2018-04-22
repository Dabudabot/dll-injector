#include "stdafx.h"
#include "injector_x64.h"

Injector64::Injector64(const STARTUPINFO startupInfo, const PROCESS_INFORMATION processInfo) : Injector(startupInfo, processInfo)
{
	m_lpDllName = L"E:\\Documents\\Visual Studio 2017\\Projects\\RE-S18\\x64\\Debug\\MyDll.dll";
}

bool Injector64::doInjection()
{
	printf("%s", "x64 injection begins\n");
	if (!getRemoteImageBase<PEB64>(m_hProcess, &m_pRemoteImageBase)										||  // NOLINT
		!findRemoteEntryPoint<IMAGE_NT_HEADERS64>(m_hProcess, m_pRemoteImageBase, &m_remoteEntryPoint)	||
		!loopEntryPoint(m_hProcess, m_processInfo.hThread, m_remoteEntryPoint, &m_originalEntryPoint)	||
		!findRemoteLoadLibrary<IMAGE_NT_HEADERS64>(m_hProcess, m_loadLibraryContext)					||
		!inject(m_hProcess, m_lpDllName, m_shellcode, m_loadLibraryContext)								||
		!deLoopEntryPoint(m_hProcess, m_remoteEntryPoint, &m_originalEntryPoint))
	{
		printf("%s", "x64 injection failed\n");
		return false;  // NOLINT
	}
	printf("%s", "x64 injection success\n");
	return true;
}

