#include "stdafx.h"
#include "injector_factory.h"
#include "injector_x64.h"
#include "injector_x86.h"


InjectorFactory::InjectorFactory(const LPCTSTR lpAppName)
{
	ZeroMemory(&m_startupInfo, sizeof(m_startupInfo));
	m_startupInfo.cb = sizeof(m_startupInfo);
	ZeroMemory(&m_processInfo, sizeof(m_processInfo));

	if (!::CreateProcess(lpAppName, 
		nullptr,
		nullptr, 
		nullptr, 
		true, 
		CREATE_SUSPENDED, 
		nullptr, 
		nullptr, 
		&m_startupInfo,
		&m_processInfo))
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}
	Sleep(1000);

	is86(m_processInfo.hProcess, m_is86);

	if (m_is86)
	{
		m_injector = std::make_unique<Injector86>(m_startupInfo, m_processInfo);
	}
	else
	{
		m_injector = std::make_unique<Injector64>(m_startupInfo, m_processInfo);
	}
}

void InjectorFactory::is86(const HANDLE hProcess, BOOL &is86)
{
	if (!IsWow64Process(hProcess, &is86))
	{
		printf("IsWow64Process failed with %lu\n", GetLastError());
	}
	printf("Remote process is %s\n", is86 ? "x86" : "x64");
}

InjectorFactory::~InjectorFactory()  // NOLINT
{
	CloseHandle(m_processInfo.hProcess);
	CloseHandle(m_processInfo.hThread);
}



