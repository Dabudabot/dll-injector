
#include "stdafx.h"
#include "detour.h"

HRESULT MyD3D11CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software,
	UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion,
	const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D11Device** ppDevice,
	D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
{
	MessageBox(nullptr, L"hook function called", L"MyDll.dll", MB_OK);
	const auto result = pfnD3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software,
		 Flags, pFeatureLevels, FeatureLevels, SDKVersion,
		 pSwapChainDesc, ppSwapChain, ppDevice,
		 pFeatureLevel, ppImmediateContext);
	return result;
}
