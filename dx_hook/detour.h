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
		         _Out_opt_ IDXGISwapChain** ppSwapChain,
		         _Out_opt_ ID3D11Device** ppDevice,
		         _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
		         _Out_opt_ ID3D11DeviceContext** ppImmediateContext
	) {

		ID3D11Device* pDevice = nullptr;
		IDXGISwapChain* pSwapChain = nullptr;
		HRESULT retValue;

		//wchar_t text[100];
		//wsprintf(text, L"D3D11CreateDeviceAndSwapChain function called hooker.m_oldBytes %x to %x", hooker.m_oldBytes, hooker.m_jmp);
		//MessageBox(nullptr, L"D3D11CreateDeviceAndSwapChain", L"MyDll.dll", MB_OK);

		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_myProtect, nullptr);			// assign read write protection
		memcpy(hooker.m_pOriginalFunction, hooker.m_oldBytes, SIZE);									// restore backup
		if (ppSwapChain == nullptr)	
		{
			//it requested only device
			retValue = ::D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software,
				Flags, pFeatureLevels, FeatureLevels, SDKVersion,
				pSwapChainDesc, ppSwapChain, &pDevice,
				pFeatureLevel, ppImmediateContext);														// get return value of original function and interface
			MessageBox(nullptr, L"D3D11CreateDeviceAndSwapChain + device", L"MyDll.dll", MB_OK);
		}
		else
		{
			//it requested device and swapchain
			retValue = ::D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software,
				Flags, pFeatureLevels, FeatureLevels, SDKVersion,
				pSwapChainDesc, &pSwapChain, &pDevice,
				pFeatureLevel, ppImmediateContext);														// get return value of original function and interface
			MessageBox(nullptr, L"D3D11CreateDeviceAndSwapChain + pSwapChain", L"MyDll.dll", MB_OK);
		}
		memcpy(hooker.m_pOriginalFunction, hooker.m_jmp, SIZE);											// set the jump instruction again
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_oldProtect, nullptr);			// reset protection

		auto overlay = new Overlay(&pDevice, ppImmediateContext);										//free it when swapchain releases

		if (pSwapChain != nullptr)
		{
			*ppSwapChain = new MyIdxgiSwapChain(&pSwapChain, overlay);
			overlay->loadContent();
		}

		*ppDevice = new MyId3D11Device(&pDevice, overlay);

		//Sleep(1000);

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
		//MessageBox(nullptr, L"D3D11CreateDevice function called 1", L"MyDll.dll", MB_OK);
		const auto retValue = ::D3D11CreateDevice(pAdapter, DriverType, Software,
			Flags, pFeatureLevels, FeatureLevels, SDKVersion,
			&pDevice, pFeatureLevel, ppImmediateContext);															// get return value of original function and interface
		memcpy(hooker.m_pOriginalFunction, hooker.m_jmp, SIZE);											// set the jump instruction again
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_oldProtect, nullptr);			// reset protection

		//MessageBox(nullptr, L"D3D11CreateDevice function called 2", L"MyDll.dll", MB_OK);
		//auto test = pDevice->GetCreationFlags();
		//wchar_t text2[100];
		//wsprintf(text2, L"device pointer %p", pDevice);
		//MessageBox(nullptr, text2, L"MyDll.dll", MB_OK);

		const auto overlay = new Overlay(&pDevice, ppImmediateContext);	//free it when swapchain releases
		//redirecting pointer to present function
		*ppDevice = new MyId3D11Device(&pDevice, overlay);

		//MessageBox(nullptr, L"D3D11CreateDevice function called", L"MyDll.dll", MB_OK);

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
		//MessageBox(nullptr, L"CreateDXGIFactory function called 1", L"MyDll.dll", MB_OK);
		void* pFactory = nullptr;

		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_myProtect, nullptr);			// assign read write protection
		memcpy(hooker.m_pOriginalFunction, hooker.m_oldBytes, SIZE);									// restore backup
		const auto retValue = ::CreateDXGIFactory(riid, &pFactory);															// get return value of original function and interface
		memcpy(hooker.m_pOriginalFunction, hooker.m_jmp, SIZE);											// set the jump instruction again
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_oldProtect, nullptr);			// reset protection
								
		*ppFactory = new MyIdxgiFactory(&pFactory);

		//MessageBox(nullptr, L"CreateDXGIFactory function called", L"MyDll.dll", MB_OK);

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

		//wchar_t text[100];
		//wsprintf(text, L"CreateDXGIFactory1 function called hooker.m_oldBytes %x to %x", hooker.m_oldBytes, hooker.m_jmp);
		//MessageBox(nullptr, text, L"MyDll.dll", MB_OK);

		//MessageBox(nullptr, L"CreateDXGIFactory1 function called 1", L"MyDll.dll", MB_OK);
		void* pFactory = nullptr;

		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_myProtect, nullptr);			// assign read write protection
		memcpy(hooker.m_pOriginalFunction, hooker.m_oldBytes, SIZE);									// restore backup
		const auto retValue = ::CreateDXGIFactory1(riid, &pFactory);									// get return value of original function and interface
		memcpy(hooker.m_pOriginalFunction, hooker.m_jmp, SIZE);											// set the jump instruction again
		VirtualProtect(LPVOID(hooker.m_pOriginalFunction), SIZE, hooker.m_oldProtect, nullptr);			// reset protection

		*ppFactory = new MyIdxgiFactory(&pFactory);

		//MessageBox(nullptr, L"CreateDXGIFactory1 function called", L"MyDll.dll", MB_OK);

		return retValue;
	}
};