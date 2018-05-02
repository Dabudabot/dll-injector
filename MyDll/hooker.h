// hooker.h : home of the hooker class
#pragma once

#define SIZE 6

/**
 * \brief Function hooking logic goes here
 */
class Hooker
{
	// Methods
public:
	/**
	 * \brief initialization of hook
	 * \param moduleName module in which original function lives
	 * \param functionName name of the original function
	 * \param pNewFunction new function to be used
	 */
	void	initHook(_In_ LPCTSTR moduleName, _In_ LPCSTR functionName, LPVOID pNewFunction);
	/**
	 * \brief releases hook
	 */
	void	unhook() const;

	// Attributes
public:
	/**
	 * \brief 6 byte jmp instruction
	 */
	BYTE			m_jmp[SIZE] = { 0 };
	/**
	 * \brief backup of the replaced bytes
	 */
	BYTE			m_oldBytes[SIZE] = { 0 };
	/**
	 * \brief original protection mode
	 */
	DWORD			m_oldProtect = PAGE_EXECUTE_READWRITE;
	/**
	 * \brief new protection mode
	 */
	DWORD			m_myProtect = PAGE_EXECUTE_READWRITE;
	/**
	 * \brief pointer to original function
	 */
	LPVOID			m_pOriginalFunction;
};

