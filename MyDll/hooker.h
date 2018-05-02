#pragma once

#define SIZE 6

class Hooker
{
	// Methods
public:
	void	initHook(_In_ LPCTSTR moduleName, _In_ LPCSTR functionName, LPVOID pNewFunction);
	void	unhook() const;

	// Attributes
public:
	BYTE			m_jmp[SIZE] = { 0 };
	BYTE			m_oldBytes[SIZE] = { 0 };
	DWORD			m_oldProtect, m_myProtect = PAGE_EXECUTE_READWRITE;
	LPVOID			m_pOriginalFunction;
};

