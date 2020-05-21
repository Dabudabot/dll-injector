// injector_factory.cpp : injector factory realization
#include "stdafx.h"
#include "injector_factory.h"

InjectorFactory::InjectorFactory(const LPCTSTR lpAppName)
{
	createProcess(lpAppName);	//create suspended process in which we need to inject
	is86();						//get is process x64 or x86
	generateInjector();			//run injector depending on x64 or x86
}

void InjectorFactory::createProcess(const LPCTSTR lpAppName)
{
	ZeroMemory(&m_startupInfo_, sizeof(m_startupInfo_));		//Zero variables
	m_startupInfo_.cb = sizeof(m_startupInfo_);
	ZeroMemory(&m_processInfo_, sizeof(m_processInfo_));

	if (!::CreateProcess(lpAppName,							//create process suspended
		nullptr,
		nullptr,
		nullptr,
		true,
		CREATE_SUSPENDED,
		nullptr,
		nullptr,
		&m_startupInfo_,
		&m_processInfo_))
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}
	Sleep(1000);											//pause to let process be created
}

void InjectorFactory::is86()
{
	if (!IsWow64Process(m_processInfo_.hProcess, &m_is86_))
	{
		printf("IsWow64Process failed with %lu\n", GetLastError());
	}
	printf("Remote process is %s\n", m_is86_ ? "x86" : "x64");
}

void InjectorFactory::generateInjector()
{	
	if (m_is86_)
	{
		m_injector_ = std::make_shared<Injector86>(m_startupInfo_, m_processInfo_);
	}
	else
	{
		m_injector_ = std::make_shared<Injector64>(m_startupInfo_, m_processInfo_);
	}
}

InjectorFactory::~InjectorFactory()  // NOLINT
{
	CloseHandle(m_processInfo_.hProcess);
	CloseHandle(m_processInfo_.hThread);
}

InjectorFactory::PINJECTOR InjectorFactory::getInjector() const
{
	return m_injector_;
}
