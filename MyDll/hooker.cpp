#include "stdafx.h"
#include "hooker.h"


Hooker::Hooker(const LPCTSTR moduleName, const LPCSTR functionName, void* pHookFunction, void* pOriginalFunction)
{
	m_hook.m_moduleName = moduleName;
	m_hook.m_functionName = functionName;
	m_hook.m_pHookFunction = pHookFunction;
	m_hook.m_pOriginalFunction = pOriginalFunction;
	m_hook.m_isHooked = false;
}


Hooker::~Hooker()
{
}

bool Hooker::initHook()
{
	char opcodes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0xe9, 0x00, 0x00, 0x00, 0x00 };

	if (m_hook.m_isHooked) {
		printf("function %s already hooked", m_hook.m_functionName);
		return false;
	}

	const auto hModule = GetModuleHandle(m_hook.m_moduleName);
	if (nullptr == hModule) {
		m_hook.m_isHooked = false;
		printf("GetModuleHandle failed with: %lu", GetLastError());
		return false;
	}

	m_hook.m_pOriginalFunction = GetProcAddress(hModule, m_hook.m_functionName);
	if (nullptr == m_hook.m_pOriginalFunction) {
		m_hook.m_isHooked = false;
		printf("GetProcAddress failed with: %lu", GetLastError());
		return false;
	}

	m_hook.m_jmp[0] = 0xe9;
	*PULONG(&m_hook.m_jmp[1]) = ULONG(m_hook.m_pHookFunction) - ULONG(m_hook.m_pOriginalFunction) - 5;

	memcpy(m_hook.m_apiBytes, m_hook.m_pOriginalFunction, 5);

	m_hook.m_pApiFunction = VirtualAlloc(nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (nullptr == m_hook.m_pApiFunction) {
		printf("VirtualAlloc failed with: %lu", GetLastError());
		return false;
	}

	memcpy(m_hook.m_pApiFunction, m_hook.m_apiBytes, 5);

	const auto origFunc = ULONG(m_hook.m_pApiFunction) + 5;
	const auto funcAddr = ULONG(m_hook.m_pOriginalFunction) + 5;

	*LPBYTE(LPBYTE(m_hook.m_pApiFunction) + 5) = 0xe9;
	*PULONG(LPBYTE(m_hook.m_pApiFunction) + 6) = ULONG(funcAddr) - ULONG(origFunc) - 5;

	m_hook.m_isHooked = true;
	return true;
}

bool Hooker::insertHook()
{
	DWORD op;
	if (!m_hook.m_isHooked) {  // NOLINT(readability-simplify-boolean-expr)
		return false;
	}
	VirtualProtect(m_hook.m_pOriginalFunction, 5, PAGE_EXECUTE_READWRITE, &op);
	memcpy(m_hook.m_pOriginalFunction, m_hook.m_jmp, 5);
	VirtualProtect(m_hook.m_pOriginalFunction, 5, op, &op);
	return true;
}

bool Hooker::unhook()
{
	DWORD op;
	if (!m_hook.m_isHooked) {
		return false;
	}
	VirtualProtect(m_hook.m_pOriginalFunction, 5, PAGE_EXECUTE_READWRITE, &op);
	memcpy(m_hook.m_pOriginalFunction, m_hook.m_apiBytes, 5);
	VirtualProtect(m_hook.m_pOriginalFunction, 5, op, &op);

	m_hook.m_isHooked = false;
	return true;
}

bool Hooker::freeHook()
{
	if (m_hook.m_isHooked) {
		return false;
	}
	VirtualFree(m_hook.m_pApiFunction, 0, MEM_RELEASE);
	return true;
}

