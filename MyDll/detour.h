// detour.h : Home of my realizations of hooked functions
// each function to be hooked should be bounded by namespace
// namespace contain pointer to function typedef, hooker class instance and detour function realisation
#pragma once

#include "stdafx.h"
#include "hooker.h"
#include "idxgiswapchain.h"
#include "id3d11device.h"
#include "idxgifactory.h"

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

		auto overlay = new Overlay(ppDevice, ppImmediateContext);	//free it when swapchain releases

		//redirecting pointer to present function
		*ppSwapChain = new MyIdxgiSwapChain(&pSwapChain, overlay);

		overlay->loadContent();
		return retValue;																				// return original return value
	}
};

namespace MyD3D11CreateDevice
{
	/**
	* \brief type of this function
	*/
	typedef HRESULT(WINAPI *pfn)(
		_In_opt_        IDXGIAdapter        *pAdapter,
		D3D_DRIVER_TYPE     DriverType,
		HMODULE             Software,
		UINT                Flags,
		_In_opt_  const D3D_FEATURE_LEVEL   *pFeatureLevels,
		UINT                FeatureLevels,
		UINT                SDKVersion,
		_Out_opt_       ID3D11Device        **ppDevice,
		_Out_opt_       D3D_FEATURE_LEVEL   *pFeatureLevel,
		_Out_opt_       ID3D11DeviceContext **ppImmediateContext
		);

	/**
	* \brief hooking logic stored here
	*/
	Hooker hooker;

	inline HRESULT MyFunction(
		_In_opt_        IDXGIAdapter        *pAdapter,
		D3D_DRIVER_TYPE     DriverType,
		HMODULE             Software,
		UINT                Flags,
		_In_opt_  const D3D_FEATURE_LEVEL   *pFeatureLevels,
		UINT                FeatureLevels,
		UINT                SDKVersion,
		_Out_opt_       ID3D11Device        **ppDevice,
		_Out_opt_       D3D_FEATURE_LEVEL   *pFeatureLevel,
		_Out_opt_       ID3D11DeviceContext **ppImmediateContext
	)
	{
		ID3D11Device* pDevice = nullptr;

		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_myProtect, nullptr);			// assign read write protection
		memcpy(hooker.m_pOriginalFunction, hooker.m_oldBytes, SIZE);									// restore backup
		const auto retValue = ::D3D11CreateDevice(pAdapter, DriverType, Software,
			Flags, pFeatureLevels, FeatureLevels, SDKVersion,
			&pDevice, pFeatureLevel, ppImmediateContext);															// get return value of original function and interface
		memcpy(hooker.m_pOriginalFunction, hooker.m_jmp, SIZE);											// set the jump instruction again
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_oldProtect, nullptr);			// reset protection

		const auto overlay = new Overlay(&pDevice, ppImmediateContext);	//free it when swapchain releases
		//redirecting pointer to present function
		*ppDevice = new MyId3D11Device(&pDevice, overlay);

		return retValue;																				// return original return value
	}
};

namespace MyCreateDxgiFactory
{
	/**
	* \brief type of this function
	*/
	typedef HRESULT(WINAPI *pfn)(
		REFIID riid,
		_Out_ void   **ppFactory
		);

	/**
	* \brief hooking logic stored here
	*/
	Hooker hooker;

	inline HRESULT MyFunction(
		REFIID riid,
		_Out_ void   **ppFactory
	)
	{
		void* pFactory = nullptr;

		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_myProtect, nullptr);			// assign read write protection
		memcpy(hooker.m_pOriginalFunction, hooker.m_oldBytes, SIZE);									// restore backup
		const auto retValue = ::CreateDXGIFactory(riid, &pFactory);															// get return value of original function and interface
		memcpy(hooker.m_pOriginalFunction, hooker.m_jmp, SIZE);											// set the jump instruction again
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_oldProtect, nullptr);			// reset protection
								
		*ppFactory = new MyIdxgiFactory(&pFactory);

		return retValue;
	}
};

namespace MyCreateDxgiFactory1
{
	/**
	* \brief type of this function
	*/
	typedef HRESULT(WINAPI *pfn)(
		REFIID riid,
		_Out_ void   **ppFactory
		);

	/**
	* \brief hooking logic stored here
	*/
	Hooker hooker;

	inline HRESULT MyFunction(
		REFIID riid,
		_Out_ void   **ppFactory
	)
	{
		void* pFactory = nullptr;

		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_myProtect, nullptr);			// assign read write protection
		memcpy(hooker.m_pOriginalFunction, hooker.m_oldBytes, SIZE);									// restore backup
		const auto retValue = ::CreateDXGIFactory1(riid, &pFactory);									// get return value of original function and interface
		memcpy(hooker.m_pOriginalFunction, hooker.m_jmp, SIZE);											// set the jump instruction again
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_oldProtect, nullptr);			// reset protection

		*ppFactory = new MyIdxgiFactory(&pFactory);

		return retValue;
	}
};