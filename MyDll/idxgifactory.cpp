#include "stdafx.h"
#include "idxgifactory.h"


MyIDXGIFactory::MyIDXGIFactory(void** ppFactory)
{
	m_pFactory_ = *ppFactory;
}

HRESULT MyIDXGIFactory::QueryInterface(const IID& riid, void** ppvObject)
{
}

ULONG MyIDXGIFactory::AddRef()
{
}

ULONG MyIDXGIFactory::Release()
{
}

HRESULT MyIDXGIFactory::SetPrivateData(const GUID& Name, UINT DataSize, const void* pData)
{
}

HRESULT MyIDXGIFactory::SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown)
{
}

HRESULT MyIDXGIFactory::GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData)
{
}

HRESULT MyIDXGIFactory::GetParent(const IID& riid, void** ppParent)
{
}

HRESULT MyIDXGIFactory::EnumAdapters(UINT Adapter, IDXGIAdapter** ppAdapter)
{
}

HRESULT MyIDXGIFactory::MakeWindowAssociation(HWND WindowHandle, UINT Flags)
{
}

HRESULT MyIDXGIFactory::GetWindowAssociation(HWND* pWindowHandle)
{
}

HRESULT MyIDXGIFactory::CreateSwapChain(IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{

}

HRESULT MyIDXGIFactory::CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter** ppAdapter)
{
}
