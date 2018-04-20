#pragma once
#include "injector_x64.h"
#include "injector_x86.h"
#include <memory>

#define PINJECTOR std::shared_ptr<Injector>

class InjectorFactory
{
	// Constructors
public:
	explicit InjectorFactory(_In_ LPCTSTR lpAppName);
	InjectorFactory(const InjectorFactory &)			= default;
	InjectorFactory(InjectorFactory &&)					= default;
	~InjectorFactory();

	// Getters
public:
	PINJECTOR		getInjector() const;

	// Methods
private:
	void			is86();
	void			createProcess(_In_ LPCTSTR lpAppName);
	void			generateInjector();

	// Attributes
private:
	BOOL						m_is86{};
	PINJECTOR					m_injector;
	STARTUPINFO					m_startupInfo{};
	PROCESS_INFORMATION			m_processInfo{};

	//Operators
public:
	InjectorFactory & InjectorFactory :: operator= (const InjectorFactory &)	= default;
	InjectorFactory & InjectorFactory :: operator= (InjectorFactory &&)			= default;
};

