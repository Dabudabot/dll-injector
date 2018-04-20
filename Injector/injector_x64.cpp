
#include "stdafx.h"
#include "injector_x64.h"

Injector64::Injector64(const STARTUPINFO startupInfo, const PROCESS_INFORMATION processInfo) : Injector()
{
	m_startupInfo = startupInfo;
	m_processInfo = processInfo;
	m_hProcess = processInfo.hProcess;
	m_lpDllName = L"\\x64\\Debug\\MyDll.dll";
}

void Injector64::doInjection(LPCTSTR dllHome)
{
	printf("Here I am");
}

