// detour.h : Home of my realizations of hooked functions
// each function to be hooked should be bounded by namespace
// namespace contain pointer to function typedef, hooker class instance and detour function realisation
#pragma once
#include "stdafx.h"
#include "hooker.h"
#include "idxgiswapchain.h"

/**
 * \brief function D3D11CreateDeviceAndSwapChain detouring
 */
namespace MyD3D11CreateDeviceAndSwapChain
{
	/**
	 * \brief type of this function
	 */
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

	/**
	 * \brief hooking logic stored here
	 */
	Hooker hooker;

	/**
	 * \brief Detour of the function
	 * \param pAdapter 
	 * \param DriverType 
	 * \param Software 
	 * \param Flags 
	 * \param pFeatureLevels 
	 * \param FeatureLevels 
	 * \param SDKVersion 
	 * \param pSwapChainDesc 
	 * \param ppSwapChain 
	 * \param ppDevice 
	 * \param pFeatureLevel 
	 * \param ppImmediateContext 
	 * \return 
	 */
	inline HRESULT MyFunction(
		_In_opt_ IDXGIAdapter* pAdapter,
		         D3D_DRIVER_TYPE DriverType,
		         HMODULE Software,
		         UINT Flags,
		         _In_opt_ const D3D_FEATURE_LEVEL* pFeatureLevels,
		         UINT FeatureLevels,
		         UINT SDKVersion,
		         _In_opt_ const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
		         _Out_opt_ MyIdxgiSwapChain** ppSwapChain,
		         _Out_opt_ ID3D11Device** ppDevice,
		         _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
		         _Out_opt_ ID3D11DeviceContext** ppImmediateContext
	) {
		IDXGISwapChain* pSwapChain = nullptr;

		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_myProtect, nullptr);			// assign read write protection
		memcpy(hooker.m_pOriginalFunction, hooker.m_oldBytes, SIZE);									// restore backup
		const auto retValue = ::D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software,
			Flags, pFeatureLevels, FeatureLevels, SDKVersion,
			pSwapChainDesc, &pSwapChain, ppDevice,
			pFeatureLevel, ppImmediateContext);															// get return value of original function and interface
		memcpy(hooker.m_pOriginalFunction, hooker.m_jmp, SIZE);											// set the jump instruction again
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_oldProtect, nullptr);			// reset protection

		auto overlay = new Overlay;	//free it when swapchain releases

		//redirecting pointer to present function
		*ppSwapChain = new MyIdxgiSwapChain(&pSwapChain, overlay);	//TODO: will it leak?

		overlay->loadContent();

		//wchar_t text[100];
		//printf("ppSwapChain %p, myIdxgiSwapChain %p, pSwapChain %p, &pSwapChain %p", ppSwapChain, myIdxgiSwapChain, pSwapChain, &pSwapChain);
		//wsprintf(text, L"ppSwapChain %p, myIdxgiSwapChain %p, pSwapChain %p, &pSwapChain %p", ppSwapChain, myIdxgiSwapChain, pSwapChain, &pSwapChain);
		//MessageBox(nullptr, text, L"MyDll.dll", MB_OK);
		MessageBox(nullptr, L"hook function called", L"MyDll.dll", MB_OK);
		return retValue;																				// return original return value
	}
};



