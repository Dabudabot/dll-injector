#include "stdafx.h"
#include "injector_factory.h"

InjectorFactory::InjectorFactory(const LPCTSTR lpAppName)
{
	createProcess(lpAppName);
	is86();
	generateInjector();
}

void InjectorFactory::createProcess(const LPCTSTR lpAppName)
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
}

void InjectorFactory::is86()
{
	if (!IsWow64Process(m_processInfo.hProcess, &m_is86))
	{
		printf("IsWow64Process failed with %lu\n", GetLastError());
	}
	printf("Remote process is %s\n", m_is86 ? "x86" : "x64");
}

void InjectorFactory::generateInjector()
{	
	if (m_is86)
	{
		m_injector = std::make_shared<Injector86>(m_startupInfo, m_processInfo);
	}
	else
	{
		m_injector = std::make_shared<Injector64>(m_startupInfo, m_processInfo);
	}
}

InjectorFactory::~InjectorFactory()  // NOLINT
{
	CloseHandle(m_processInfo.hProcess);
	CloseHandle(m_processInfo.hThread);
}

InjectorFactory::PINJECTOR InjectorFactory::getInjector() const
{
	return m_injector;
}
