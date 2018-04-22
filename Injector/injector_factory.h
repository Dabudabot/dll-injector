#pragma once
#include "injector_x64.h"
#include "injector_x86.h"
#include <memory>
/**
 * \brief InjectorFactory class generate injector 
 * which can be accessed using getInjector function
 * To generate appropriate injector it has to create suspended 
 * process and pass handle to the created injector
 * 
 * Injector choose depends on x64 or x86 version of victim
 */
class InjectorFactory
{
	// Typedefs
public:
	/**
	* \brief just to make declaration of shared ptr shorter
	*/
	typedef std::shared_ptr<Injector> PINJECTOR;

	// Constructors
public:
	/**
	 * \brief non-default ctor manages the algorithm of 
	 * creation process and injector generation
	 * \param lpAppName victim app name
	 */
	explicit InjectorFactory(_In_ LPCTSTR lpAppName);

	/**
	 * \brief default implementation of copy ctor,
	 * this class is copyable
	 */
	InjectorFactory(_In_ const InjectorFactory &)				= default;

	/**
	 * \brief default implementation of move ctor
	 * this class is movable
	 */
	InjectorFactory(_In_ InjectorFactory &&)					= default;

	/**
	 * \brief dtor which closes handles
	 * no need to destroy pointer to injector
	 * because of the usage of the shared_ptr
	 */
	~InjectorFactory();

	// Getters
public:
	/**
	 * \brief to access generated injector
	 * \return shared pointer to injector 
	 * depending on the input to ctor
	 */
	PINJECTOR		getInjector() const;

	// Methods
private:
	/**
	 * \brief to get is m_processInfo x64 or x86
	 * store answer in m_is64
	 */
	void			is86();
	/**
	 * \brief to create a process suspended
	 * \param lpAppName name of the app
	 */
	void			createProcess(_In_ LPCTSTR lpAppName);
	/**
	 * \brief to get witch injector to use further
	 */
	void			generateInjector();

	// Attributes
private:
	BOOL						m_is86{};
	PINJECTOR					m_injector;
	STARTUPINFO					m_startupInfo{};
	PROCESS_INFORMATION			m_processInfo{};

	//Operators
public:
	/**
	 * \brief default implemetation of copy operator
	 * \return 
	 */
	InjectorFactory & InjectorFactory :: operator= (const InjectorFactory &)	= default;
	/**
	 * \brief default implemetation of move operator
	 * \return 
	 */
	InjectorFactory & InjectorFactory :: operator= (InjectorFactory &&)			= default;
};

