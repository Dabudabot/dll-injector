// hooker.cpp : implementation of hooker.h
#include "stdafx.h"
#include "hooker.h"

void Hooker::initHook(const LPCTSTR moduleName, const LPCSTR functionName, LPVOID pNewFunction)
{
	m_pOriginalFunction = GetProcAddress(GetModuleHandle(moduleName), functionName);			//get adress of the original function

	if (m_pOriginalFunction == nullptr)															//in case of fail do not continue
	{
		MessageBox(nullptr, L"Original function is null", L"MyDll.dll", MB_OK);
		return;
	}

	BYTE tempJmp[SIZE] = { 0xE9, 0x90, 0x90, 0x90, 0x90, 0xC3 };								// 0xE9 = JMP 0x90 = NOP 0xC3 = RET
	memcpy(m_jmp, tempJmp, SIZE);																// store jmp instruction to m_jmp
	auto jmpSize = (DWORD(pNewFunction) - DWORD(m_pOriginalFunction) - 5);						// calculate jump distance
	VirtualProtect(LPVOID(m_pOriginalFunction), SIZE,											// assign read write protection
		PAGE_EXECUTE_READWRITE, &m_oldProtect);
	memcpy(m_oldBytes, m_pOriginalFunction, SIZE);												// make backup
	memcpy(&m_jmp[1], &jmpSize, 4);																// fill the nop's with the jump distance (JMP,distance(4bytes),RET)
	memcpy(m_pOriginalFunction, m_jmp, SIZE);													// set jump instruction at the beginning of the original function
	VirtualProtect(LPVOID(m_pOriginalFunction), SIZE, m_oldProtect, nullptr);					// reset protection
}

void Hooker::unhook() const
{
	memcpy(m_pOriginalFunction, m_oldBytes, SIZE);												// restore original function
}

