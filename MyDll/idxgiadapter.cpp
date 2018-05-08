#include "stdafx.h"
#include "idxgiadapter.h"

MyIDXGIAdapter::MyIDXGIAdapter(IDXGIAdapter4** ppvObject)
{
	m_idxgiAdapter4_ = *ppvObject;
}

HRESULT MyIDXGIAdapter::QueryInterface(const IID& riid, void** ppvObject)
{
	return m_idxgiAdapter4_->QueryInterface(riid, ppvObject);
}

ULONG MyIDXGIAdapter::AddRef()
{
	return m_idxgiAdapter4_->AddRef();
}

ULONG MyIDXGIAdapter::Release()
{
	free(m_factory_);
	return m_idxgiAdapter4_->Release();
}

HRESULT MyIDXGIAdapter::SetPrivateData(const GUID& Name, UINT DataSize, const void* pData)
{
	return m_idxgiAdapter4_->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIDXGIAdapter::SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown)
{
	return m_idxgiAdapter4_->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIDXGIAdapter::GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData)
{
	return m_idxgiAdapter4_->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIDXGIAdapter::GetParent(const IID& riid, void** ppParent)
{
	const auto result = m_idxgiAdapter4_->GetParent(riid, ppParent);
	if (riid == __uuidof(IDXGIFactory) || 
		riid == __uuidof(IDXGIFactory1) || 
		riid == __uuidof(IDXGIFactory2))
	{
		m_factory_ = new MyIDXGIFactory(ppParent);
		ppParent = reinterpret_cast<void**>(&m_factory_);
	}
	return result;
}

HRESULT MyIDXGIAdapter::EnumOutputs(UINT Output, IDXGIOutput** ppOutput)
{
	return m_idxgiAdapter4_->EnumOutputs(Output, ppOutput);
}

HRESULT MyIDXGIAdapter::GetDesc(DXGI_ADAPTER_DESC* pDesc)
{
	return m_idxgiAdapter4_->GetDesc(pDesc);
}

HRESULT MyIDXGIAdapter::CheckInterfaceSupport(const GUID& InterfaceName, LARGE_INTEGER* pUMDVersion)
{
	return m_idxgiAdapter4_->CheckInterfaceSupport(InterfaceName, pUMDVersion);
}

HRESULT MyIDXGIAdapter::GetDesc1(DXGI_ADAPTER_DESC1* pDesc)
{
	return m_idxgiAdapter4_->GetDesc1(pDesc);
}

HRESULT MyIDXGIAdapter::GetDesc2(DXGI_ADAPTER_DESC2* pDesc)
{
	return m_idxgiAdapter4_->GetDesc2(pDesc);
}

HRESULT MyIDXGIAdapter::RegisterHardwareContentProtectionTeardownStatusEvent(HANDLE hEvent, DWORD* pdwCookie)
{
	return m_idxgiAdapter4_->RegisterHardwareContentProtectionTeardownStatusEvent(hEvent, pdwCookie);
}

void MyIDXGIAdapter::UnregisterHardwareContentProtectionTeardownStatus(DWORD dwCookie)
{
	return m_idxgiAdapter4_->UnregisterHardwareContentProtectionTeardownStatus(dwCookie);
}

HRESULT MyIDXGIAdapter::QueryVideoMemoryInfo(UINT NodeIndex, DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup,
	DXGI_QUERY_VIDEO_MEMORY_INFO* pVideoMemoryInfo)
{
	return m_idxgiAdapter4_->QueryVideoMemoryInfo(NodeIndex, MemorySegmentGroup, pVideoMemoryInfo);
}

HRESULT MyIDXGIAdapter::SetVideoMemoryReservation(UINT NodeIndex, DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup,
	UINT64 Reservation)
{
	return m_idxgiAdapter4_->SetVideoMemoryReservation(NodeIndex, MemorySegmentGroup, Reservation);
}

HRESULT MyIDXGIAdapter::RegisterVideoMemoryBudgetChangeNotificationEvent(HANDLE hEvent, DWORD* pdwCookie)
{
	return m_idxgiAdapter4_->RegisterVideoMemoryBudgetChangeNotificationEvent(hEvent, pdwCookie);
}

void MyIDXGIAdapter::UnregisterVideoMemoryBudgetChangeNotification(DWORD dwCookie)
{
	return m_idxgiAdapter4_->UnregisterVideoMemoryBudgetChangeNotification(dwCookie);
}

HRESULT MyIDXGIAdapter::GetDesc3(DXGI_ADAPTER_DESC3* pDesc)
{
	return m_idxgiAdapter4_->GetDesc3(pDesc);
}
