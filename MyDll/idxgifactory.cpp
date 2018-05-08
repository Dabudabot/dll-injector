#include "stdafx.h"
#include "idxgifactory.h"

MyIDXGIFactory::MyIDXGIFactory(void** ppFactory)
{
	m_pFactory_ = static_cast<IDXGIFactory2*>(*ppFactory);
}

HRESULT MyIDXGIFactory::QueryInterface(const IID& riid, void** ppvObject)
{
	return m_pFactory_->QueryInterface(riid, ppvObject);
}

ULONG MyIDXGIFactory::AddRef()
{
	return m_pFactory_->AddRef();
}

ULONG MyIDXGIFactory::Release()
{
	return m_pFactory_->Release();
}

HRESULT MyIDXGIFactory::SetPrivateData(const GUID& Name, UINT DataSize, const void* pData)
{
	return m_pFactory_->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIDXGIFactory::SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown)
{
	return m_pFactory_->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIDXGIFactory::GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData)
{
	return m_pFactory_->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIDXGIFactory::GetParent(const IID& riid, void** ppParent)
{
	return m_pFactory_->GetParent(riid, ppParent);
}

HRESULT MyIDXGIFactory::EnumAdapters(UINT Adapter, IDXGIAdapter** ppAdapter)
{
	return m_pFactory_->EnumAdapters(Adapter, ppAdapter);
}

HRESULT MyIDXGIFactory::MakeWindowAssociation(HWND WindowHandle, UINT Flags)
{
	return m_pFactory_->MakeWindowAssociation(WindowHandle, Flags);
}

HRESULT MyIDXGIFactory::GetWindowAssociation(HWND* pWindowHandle)
{
	return m_pFactory_->GetWindowAssociation(pWindowHandle);
}

HRESULT MyIDXGIFactory::CreateSwapChain(IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{
	const auto result = m_pFactory_->CreateSwapChain(pDevice, pDesc, ppSwapChain);
	auto overlay = static_cast<MyID3D11Device*>(pDevice)->m_overlay;
	*ppSwapChain = new MyIdxgiSwapChain(ppSwapChain, overlay);
	overlay->loadContent();
	return result;
}

HRESULT MyIDXGIFactory::CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter** ppAdapter)
{
	return m_pFactory_->CreateSoftwareAdapter(Module, ppAdapter);
}

HRESULT MyIDXGIFactory::EnumAdapters1(UINT Adapter, IDXGIAdapter1** ppAdapter)
{
	return m_pFactory_->EnumAdapters1(Adapter, ppAdapter);
}

BOOL MyIDXGIFactory::IsCurrent()
{
	return m_pFactory_->IsCurrent();
}

BOOL MyIDXGIFactory::IsWindowedStereoEnabled()
{
	return m_pFactory_->IsWindowedStereoEnabled();
}

HRESULT MyIDXGIFactory::CreateSwapChainForHwnd(IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc,
	const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
	const auto result = m_pFactory_->CreateSwapChainForHwnd(pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
	auto overlay = static_cast<MyID3D11Device*>(pDevice)->m_overlay;
	*ppSwapChain = new MyIdxgiSwapChain(ppSwapChain, overlay);
	overlay->loadContent();
	return result;
}

HRESULT MyIDXGIFactory::CreateSwapChainForCoreWindow(IUnknown* pDevice, IUnknown* pWindow,
	const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
	const auto result = m_pFactory_->CreateSwapChainForCoreWindow(pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
	auto overlay = static_cast<MyID3D11Device*>(pDevice)->m_overlay;
	*ppSwapChain = new MyIdxgiSwapChain(ppSwapChain, overlay);
	overlay->loadContent();
	return result;
}

HRESULT MyIDXGIFactory::GetSharedResourceAdapterLuid(HANDLE hResource, LUID* pLuid)
{
	return m_pFactory_->GetSharedResourceAdapterLuid(hResource, pLuid);
}

HRESULT MyIDXGIFactory::RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg, DWORD* pdwCookie)
{
	return m_pFactory_->RegisterStereoStatusWindow(WindowHandle, wMsg, pdwCookie);
}

HRESULT MyIDXGIFactory::RegisterStereoStatusEvent(HANDLE hEvent, DWORD* pdwCookie)
{
	return m_pFactory_->RegisterStereoStatusEvent(hEvent, pdwCookie);
}

void MyIDXGIFactory::UnregisterStereoStatus(DWORD dwCookie)
{
	return m_pFactory_->UnregisterStereoStatus(dwCookie);
}

HRESULT MyIDXGIFactory::RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg, DWORD* pdwCookie)
{
	return m_pFactory_->RegisterOcclusionStatusWindow(WindowHandle, wMsg, pdwCookie);
}

HRESULT MyIDXGIFactory::RegisterOcclusionStatusEvent(HANDLE hEvent, DWORD* pdwCookie)
{
	return m_pFactory_->RegisterOcclusionStatusEvent(hEvent, pdwCookie);
}

void MyIDXGIFactory::UnregisterOcclusionStatus(DWORD dwCookie)
{
	return m_pFactory_->UnregisterOcclusionStatus(dwCookie);
}

HRESULT MyIDXGIFactory::CreateSwapChainForComposition(IUnknown* pDevice, const DXGI_SWAP_CHAIN_DESC1* pDesc,
	IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
	const auto result = m_pFactory_->CreateSwapChainForComposition(pDevice, pDesc, pRestrictToOutput, ppSwapChain);
	auto overlay = static_cast<MyID3D11Device*>(pDevice)->m_overlay;
	*ppSwapChain = new MyIdxgiSwapChain(ppSwapChain, overlay);
	overlay->loadContent();
	return result;
}
