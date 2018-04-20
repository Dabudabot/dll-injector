#pragma once
#include "injector.h"
#include <memory>

class InjectorFactory
{
	// Constructors
public:
	explicit InjectorFactory(_In_ LPCTSTR lpAppName);
	~InjectorFactory();

	//Methods
public:
	static void is86(_In_ HANDLE hProcess, _Out_ BOOL &is86);

	// Attributes
public:
	BOOL						m_is86{};
	std::unique_ptr<Injector>	m_injector{};
	STARTUPINFO					m_startupInfo{};
	PROCESS_INFORMATION			m_processInfo{};
};

