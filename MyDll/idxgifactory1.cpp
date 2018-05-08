#include "stdafx.h"
#include "idxgifactory1.h"
#include "idxgiswapchain.h"
#include "id3d11device.h"


MyIDXGIFactory1::MyIDXGIFactory1(void** ppFactory)
{
	m_pFactory_ = static_cast<IDXGIFactory1*>(*ppFactory);
}

HRESULT MyIDXGIFactory1::QueryInterface(const IID& riid, void** ppvObject)
{
	return m_pFactory_->QueryInterface(riid, ppvObject);
}

ULONG MyIDXGIFactory1::AddRef()
{
	return m_pFactory_->AddRef();
}

ULONG MyIDXGIFactory1::Release()
{
	return m_pFactory_->Release();
}

HRESULT MyIDXGIFactory1::SetPrivateData(const GUID& Name, UINT DataSize, const void* pData)
{
	return m_pFactory_->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIDXGIFactory1::SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown)
{
	return m_pFactory_->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIDXGIFactory1::GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData)
{
	return m_pFactory_->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIDXGIFactory1::GetParent(const IID& riid, void** ppParent)
{
	return m_pFactory_->GetParent(riid, ppParent);
}

HRESULT MyIDXGIFactory1::EnumAdapters(UINT Adapter, IDXGIAdapter** ppAdapter)
{
	return m_pFactory_->EnumAdapters(Adapter, ppAdapter);
}

HRESULT MyIDXGIFactory1::MakeWindowAssociation(HWND WindowHandle, UINT Flags)
{
	return m_pFactory_->MakeWindowAssociation(WindowHandle, Flags);
}

HRESULT MyIDXGIFactory1::GetWindowAssociation(HWND* pWindowHandle)
{
	return m_pFactory_->GetWindowAssociation(pWindowHandle);
}

HRESULT MyIDXGIFactory1::CreateSwapChain(IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{
	const auto result = m_pFactory_->CreateSwapChain(pDevice, pDesc, ppSwapChain);
	auto overlay = static_cast<MyID3D11Device*>(pDevice)->m_overlay;
	*ppSwapChain = new MyIdxgiSwapChain(ppSwapChain, overlay);
	overlay->loadContent();
	return result;
}

HRESULT MyIDXGIFactory1::CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter** ppAdapter)
{
	return m_pFactory_->CreateSoftwareAdapter(Module, ppAdapter);
}

HRESULT MyIDXGIFactory1::EnumAdapters1(UINT Adapter, IDXGIAdapter1** ppAdapter)
{
	return m_pFactory_->EnumAdapters1(Adapter, ppAdapter);
}

BOOL MyIDXGIFactory1::IsCurrent()
{
	return m_pFactory_->IsCurrent();
}
