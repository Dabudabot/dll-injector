#include "stdafx.h"
#include "idxgifactory.h"
#include "id3d11device.h"
#include "idxgiswapchain.h"

MyIdxgiFactory::MyIdxgiFactory(void** ppFactory)
{
	m_pFactory_ = static_cast<IDXGIFactory2*>(*ppFactory);
}

HRESULT MyIdxgiFactory::QueryInterface(const IID& riid, void** ppvObject)
{
	return m_pFactory_->QueryInterface(riid, ppvObject);
}

ULONG MyIdxgiFactory::AddRef()
{
	return m_pFactory_->AddRef();
}

ULONG MyIdxgiFactory::Release()
{
	return m_pFactory_->Release();
}

HRESULT MyIdxgiFactory::SetPrivateData(const GUID& Name, UINT DataSize, const void* pData)
{
	return m_pFactory_->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIdxgiFactory::SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown)
{
	return m_pFactory_->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIdxgiFactory::GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData)
{
	return m_pFactory_->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIdxgiFactory::GetParent(const IID& riid, void** ppParent)
{
	return m_pFactory_->GetParent(riid, ppParent);
}

HRESULT MyIdxgiFactory::EnumAdapters(UINT Adapter, IDXGIAdapter** ppAdapter)
{
	return m_pFactory_->EnumAdapters(Adapter, ppAdapter);
}

HRESULT MyIdxgiFactory::MakeWindowAssociation(HWND WindowHandle, UINT Flags)
{
	return m_pFactory_->MakeWindowAssociation(WindowHandle, Flags);
}

HRESULT MyIdxgiFactory::GetWindowAssociation(HWND* pWindowHandle)
{
	return m_pFactory_->GetWindowAssociation(pWindowHandle);
}

HRESULT MyIdxgiFactory::CreateSwapChain(IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{
	MessageBox(nullptr, L"CreateSwapChain function called", L"MyDll.dll", MB_OK);
	const auto result = m_pFactory_->CreateSwapChain(pDevice, pDesc, ppSwapChain);
	auto overlay = static_cast<MyId3D11Device*>(pDevice)->m_overlay;
	*ppSwapChain = new MyIdxgiSwapChain(ppSwapChain, overlay);
	overlay->loadContent();
	return result;
}

HRESULT MyIdxgiFactory::CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter** ppAdapter)
{
	return m_pFactory_->CreateSoftwareAdapter(Module, ppAdapter);
}

HRESULT MyIdxgiFactory::EnumAdapters1(UINT Adapter, IDXGIAdapter1** ppAdapter)
{
	return m_pFactory_->EnumAdapters1(Adapter, ppAdapter);
}

BOOL MyIdxgiFactory::IsCurrent()
{
	return m_pFactory_->IsCurrent();
}

BOOL MyIdxgiFactory::IsWindowedStereoEnabled()
{
	return m_pFactory_->IsWindowedStereoEnabled();
}

HRESULT MyIdxgiFactory::CreateSwapChainForHwnd(IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc,
	const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
	MessageBox(nullptr, L"CreateSwapChainForHwnd function called", L"MyDll.dll", MB_OK);
	const auto result = m_pFactory_->CreateSwapChainForHwnd(pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
	auto overlay = static_cast<MyId3D11Device*>(pDevice)->m_overlay;
	*ppSwapChain = new MyIdxgiSwapChain(ppSwapChain, overlay);
	overlay->loadContent();
	return result;
}

HRESULT MyIdxgiFactory::CreateSwapChainForCoreWindow(IUnknown* pDevice, IUnknown* pWindow,
	const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
	MessageBox(nullptr, L"CreateSwapChainForCoreWindow function called", L"MyDll.dll", MB_OK);
	const auto result = m_pFactory_->CreateSwapChainForCoreWindow(pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
	auto overlay = static_cast<MyId3D11Device*>(pDevice)->m_overlay;
	*ppSwapChain = new MyIdxgiSwapChain(ppSwapChain, overlay);
	overlay->loadContent();
	return result;
}

HRESULT MyIdxgiFactory::GetSharedResourceAdapterLuid(HANDLE hResource, LUID* pLuid)
{
	return m_pFactory_->GetSharedResourceAdapterLuid(hResource, pLuid);
}

HRESULT MyIdxgiFactory::RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg, DWORD* pdwCookie)
{
	return m_pFactory_->RegisterStereoStatusWindow(WindowHandle, wMsg, pdwCookie);
}

HRESULT MyIdxgiFactory::RegisterStereoStatusEvent(HANDLE hEvent, DWORD* pdwCookie)
{
	return m_pFactory_->RegisterStereoStatusEvent(hEvent, pdwCookie);
}

void MyIdxgiFactory::UnregisterStereoStatus(DWORD dwCookie)
{
	return m_pFactory_->UnregisterStereoStatus(dwCookie);
}

HRESULT MyIdxgiFactory::RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg, DWORD* pdwCookie)
{
	return m_pFactory_->RegisterOcclusionStatusWindow(WindowHandle, wMsg, pdwCookie);
}

HRESULT MyIdxgiFactory::RegisterOcclusionStatusEvent(HANDLE hEvent, DWORD* pdwCookie)
{
	return m_pFactory_->RegisterOcclusionStatusEvent(hEvent, pdwCookie);
}

void MyIdxgiFactory::UnregisterOcclusionStatus(DWORD dwCookie)
{
	return m_pFactory_->UnregisterOcclusionStatus(dwCookie);
}

HRESULT MyIdxgiFactory::CreateSwapChainForComposition(IUnknown* pDevice, const DXGI_SWAP_CHAIN_DESC1* pDesc,
	IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
	MessageBox(nullptr, L"CreateSwapChainForComposition function called", L"MyDll.dll", MB_OK);
	const auto result = m_pFactory_->CreateSwapChainForComposition(pDevice, pDesc, pRestrictToOutput, ppSwapChain);
	auto overlay = static_cast<MyId3D11Device*>(pDevice)->m_overlay;
	*ppSwapChain = new MyIdxgiSwapChain(ppSwapChain, overlay);
	overlay->loadContent();
	return result;
}
