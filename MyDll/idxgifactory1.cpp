#include "stdafx.h"
#include "idxgifactory1.h"


MyIDXGIFactory1::MyIDXGIFactory1(void** ppFactory)
{
	m_pFactory_ = *ppFactory;
}

HRESULT MyIDXGIFactory1::QueryInterface(const IID& riid, void** ppvObject)
{
}

ULONG MyIDXGIFactory1::AddRef()
{
}

ULONG MyIDXGIFactory1::Release()
{
}

HRESULT MyIDXGIFactory1::SetPrivateData(const GUID& Name, UINT DataSize, const void* pData)
{
}

HRESULT MyIDXGIFactory1::SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown)
{
}

HRESULT MyIDXGIFactory1::GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData)
{
}

HRESULT MyIDXGIFactory1::GetParent(const IID& riid, void** ppParent)
{
}

HRESULT MyIDXGIFactory1::EnumAdapters(UINT Adapter, IDXGIAdapter** ppAdapter)
{
}

HRESULT MyIDXGIFactory1::MakeWindowAssociation(HWND WindowHandle, UINT Flags)
{
}

HRESULT MyIDXGIFactory1::GetWindowAssociation(HWND* pWindowHandle)
{
}

HRESULT MyIDXGIFactory1::CreateSwapChain(IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{
}

HRESULT MyIDXGIFactory1::CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter** ppAdapter)
{
}

HRESULT MyIDXGIFactory1::EnumAdapters1(UINT Adapter, IDXGIAdapter1** ppAdapter)
{
}

BOOL MyIDXGIFactory1::IsCurrent()
{
}
