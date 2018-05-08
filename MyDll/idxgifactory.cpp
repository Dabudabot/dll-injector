#include "stdafx.h"
#include "idxgifactory.h"
#include "idxgiswapchain.h"
#include "id3d11device.h"


MyIDXGIFactory::MyIDXGIFactory(void** ppFactory)
{
	m_pFactory_ = static_cast<IDXGIFactory*>(*ppFactory);
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
