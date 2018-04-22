#pragma once
#include "stdafx.h"
#include "injector.h"

class Injector64 : public Injector
{
	// Constructors
public:
	Injector64(_In_ STARTUPINFO startupInfo, _In_ PROCESS_INFORMATION processInfo);
	~Injector64();

	// Methods
public:

	// Overrides
public:
	bool		doInjection()			override;
	bool		findRemoteEntryPoint()	override;
	bool		getRemoteImageBase()	override;
	bool		loopEntryPoint()		override;
	bool		deLoopEntryPoint()		override;
	bool		findLocalPeHeader()		override;
	bool		findRemoteLoadLibrary()	override;
	bool		inject()				override;
	bool		findExport()			override;

	// Attributes
public:
	PIMAGE_NT_HEADERS64		m_pLocalPeHeader;

	const UCHAR g_shellcode_x64[56] =	//TODO why it requires size?
	{
		/*0x00:*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//pLoadLibrary pointer, RUNTIME
		/*0x08:*/ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,   //8x nops to fix disassembly of VS
		/*0x10:*/ 0x48, 0x83, 0xEC, 0x28,							//sub         rsp,28h
		/*0x14:*/ 0x48, 0x8D, 0x0D, 0x1D, 0x00, 0x00, 0x00,			//lea         rcx,[RIP+(38h-1Bh)]
		/*0x1B:*/ 0xFF, 0x15, 0xDF, 0xFF, 0xFF, 0xFF,				//call        qword ptr[RIP-(21h-0)]
		/*0x21:*/ 0x33, 0xC9,										//xor         ecx, ecx
		/*0x23:*/ 0x83, 0xCA, 0xFF,									//or          edx, 0FFFFFFFFh
		/*0x26:*/ 0x48, 0x85, 0xC0,									//test        rax, rax
		/*0x29:*/ 0x0F, 0x44, 0xCA,									//cmove       ecx, edx
		/*0x2C:*/ 0x8B, 0xC1,										//mov         eax, ecx
		/*0x2E:*/ 0x48, 0x83, 0xC4, 0x28,							//add         rsp, 28h
		/*0x32:*/ 0xC3,												//ret
		/*0x33:*/ 0x90, 0x90, 0x90, 0x90, 0x90						//5x nop for alignment
		/*0x38:*/													//String: "MyDll.dll"
	};
};
