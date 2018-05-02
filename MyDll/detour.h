#pragma once
#include "stdafx.h"
#include "hooker.h"

namespace MyD3D11CreateDeviceAndSwapChain
{
	typedef HRESULT(WINAPI *pfn)(
		_In_opt_        IDXGIAdapter         *pAdapter,
		D3D_DRIVER_TYPE      DriverType,
		HMODULE              Software,
		UINT                 Flags,
		_In_opt_  const D3D_FEATURE_LEVEL    *pFeatureLevels,
		UINT                 FeatureLevels,
		UINT                 SDKVersion,
		_In_opt_  const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
		_Out_opt_       IDXGISwapChain       **ppSwapChain,
		_Out_opt_       ID3D11Device         **ppDevice,
		_Out_opt_       D3D_FEATURE_LEVEL    *pFeatureLevel,
		_Out_opt_       ID3D11DeviceContext  **ppImmediateContext
		);

	Hooker hooker;

	inline HRESULT MyFunction(
		_In_opt_        IDXGIAdapter         *pAdapter,
		D3D_DRIVER_TYPE      DriverType,
		HMODULE              Software,
		UINT                 Flags,
		_In_opt_  const D3D_FEATURE_LEVEL    *pFeatureLevels,
		UINT                 FeatureLevels,
		UINT                 SDKVersion,
		_In_opt_  const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
		_Out_opt_       IDXGISwapChain       **ppSwapChain,
		_Out_opt_       ID3D11Device         **ppDevice,
		_Out_opt_       D3D_FEATURE_LEVEL    *pFeatureLevel,
		_Out_opt_       ID3D11DeviceContext  **ppImmediateContext
	) {
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_myProtect, nullptr);     // assign read write protection
		memcpy(hooker.m_pOriginalFunction, hooker.m_oldBytes, SIZE);                            // restore backup
		const auto retValue = ::D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software,
			Flags, pFeatureLevels, FeatureLevels, SDKVersion,
			pSwapChainDesc, ppSwapChain, ppDevice,
			pFeatureLevel, ppImmediateContext);       // get return value of original function
		memcpy(hooker.m_pOriginalFunction, hooker.m_jmp, SIZE);                                 // set the jump instruction again
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_oldProtect, nullptr);    // reset protection



		MessageBox(nullptr, L"hook function called", L"MyDll.dll", MB_OK);
		return retValue;                                                   // return original return value
	}
};



