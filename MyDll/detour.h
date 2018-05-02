#pragma once
#include "stdafx.h"
namespace detour
{
	typedef HRESULT(WINAPI *pfn_D3D11CreateDeviceAndSwapChain)(
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

	pfn_D3D11CreateDeviceAndSwapChain pfnD3D11CreateDeviceAndSwapChain;

	HRESULT MyD3D11CreateDeviceAndSwapChain(
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
		const auto result = pfnD3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software,
			Flags, pFeatureLevels, FeatureLevels, SDKVersion,
			pSwapChainDesc, ppSwapChain, ppDevice,
			pFeatureLevel, ppImmediateContext);
		MessageBox(nullptr, L"hook function called", L"MyDll.dll", MB_OK);
		return result;
	}
};



