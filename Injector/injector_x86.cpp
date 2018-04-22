
#include "stdafx.h"
#include "injector_x86.h"

Injector86::Injector86(const STARTUPINFO startupInfo, const PROCESS_INFORMATION processInfo) : Injector()
{
	m_startupInfo = startupInfo;
	m_processInfo = processInfo;
	m_hProcess = processInfo.hProcess;
	m_lpDllName = L"E:\\Documents\\Visual Studio 2017\\Projects\\RE-S18\\Debug\\MyDll.dll";
}

bool Injector86::doInjection()
{
	return false;
}

bool Injector86::findRemoteEntryPoint()
{
	return false;
}

bool Injector86::getRemoteImageBase()
{
	return false;
}

bool Injector86::loopEntryPoint()
{
	return false;
}

bool Injector86::deLoopEntryPoint()
{
	return false;
}

PIMAGE_NT_HEADERS Injector86::findLocalPeHeader(ULONG_PTR base)
{
	return nullptr;
}

bool Injector86::findRemoteLoadLibrary()
{
	return false;
}

bool Injector86::inject()
{
	return false;
}

bool Injector86::findExport(ULONG_PTR pRemoteImageBase)
{
	return false;
}
