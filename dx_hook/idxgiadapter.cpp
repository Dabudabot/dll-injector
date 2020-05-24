#include "stdafx.h"
#include "idxgiadapter.h"

MyIdxgiAdapter::MyIdxgiAdapter(IDXGIAdapter4** ppvObject)
{
	m_idxgiAdapter4_ = *ppvObject;
}

HRESULT MyIdxgiAdapter::QueryInterface(const IID& riid, void** ppvObject)
{
	return m_idxgiAdapter4_->QueryInterface(riid, ppvObject);
}

ULONG MyIdxgiAdapter::AddRef()
{
	return m_idxgiAdapter4_->AddRef();
}

ULONG MyIdxgiAdapter::Release()
{
	if (m_factory_ != nullptr)
	{
		m_factory_->Release();
		free(m_factory_);
	}
	
	return m_idxgiAdapter4_->Release();
}

HRESULT MyIdxgiAdapter::SetPrivateData(const GUID& Name, UINT DataSize, const void* pData)
{
	return m_idxgiAdapter4_->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIdxgiAdapter::SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown)
{
	return m_idxgiAdapter4_->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIdxgiAdapter::GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData)
{
	return m_idxgiAdapter4_->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIdxgiAdapter::GetParent(const IID& riid, void** ppParent)
{
	wchar_t text[100];
	wsprintf(text, L"MyIdxgiAdapter::GetParent function called with %d, IDXGIFactory = %d, IDXGIFactory1 = %d, IDXGIFactory2 = %d",
		riid, __uuidof(IDXGIFactory), __uuidof(IDXGIFactory1), __uuidof(IDXGIFactory2));
	MessageBox(nullptr, text, L"MyDll.dll", MB_OK);

	const auto result = m_idxgiAdapter4_->GetParent(riid, ppParent);
	if (riid == __uuidof(IDXGIFactory) || 
		riid == __uuidof(IDXGIFactory1) || 
		riid == __uuidof(IDXGIFactory2))
	{
		m_factory_ = new MyIdxgiFactory(ppParent);
		auto a = static_cast<void*>(m_factory_);
		ppParent = &a;
		//ppParent = reinterpret_cast<void**>(&m_factory_);
	}
	return result;
}

HRESULT MyIdxgiAdapter::EnumOutputs(UINT Output, IDXGIOutput** ppOutput)
{
	return m_idxgiAdapter4_->EnumOutputs(Output, ppOutput);
}

HRESULT MyIdxgiAdapter::GetDesc(DXGI_ADAPTER_DESC* pDesc)
{
	return m_idxgiAdapter4_->GetDesc(pDesc);
}

HRESULT MyIdxgiAdapter::CheckInterfaceSupport(const GUID& InterfaceName, LARGE_INTEGER* pUMDVersion)
{
	return m_idxgiAdapter4_->CheckInterfaceSupport(InterfaceName, pUMDVersion);
}

HRESULT MyIdxgiAdapter::GetDesc1(DXGI_ADAPTER_DESC1* pDesc)
{
	return m_idxgiAdapter4_->GetDesc1(pDesc);
}

HRESULT MyIdxgiAdapter::GetDesc2(DXGI_ADAPTER_DESC2* pDesc)
{
	return m_idxgiAdapter4_->GetDesc2(pDesc);
}

HRESULT MyIdxgiAdapter::RegisterHardwareContentProtectionTeardownStatusEvent(HANDLE hEvent, DWORD* pdwCookie)
{
	return m_idxgiAdapter4_->RegisterHardwareContentProtectionTeardownStatusEvent(hEvent, pdwCookie);
}

void MyIdxgiAdapter::UnregisterHardwareContentProtectionTeardownStatus(DWORD dwCookie)
{
	return m_idxgiAdapter4_->UnregisterHardwareContentProtectionTeardownStatus(dwCookie);
}

HRESULT MyIdxgiAdapter::QueryVideoMemoryInfo(UINT NodeIndex, DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup,
	DXGI_QUERY_VIDEO_MEMORY_INFO* pVideoMemoryInfo)
{
	return m_idxgiAdapter4_->QueryVideoMemoryInfo(NodeIndex, MemorySegmentGroup, pVideoMemoryInfo);
}

HRESULT MyIdxgiAdapter::SetVideoMemoryReservation(UINT NodeIndex, DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup,
	UINT64 Reservation)
{
	return m_idxgiAdapter4_->SetVideoMemoryReservation(NodeIndex, MemorySegmentGroup, Reservation);
}

HRESULT MyIdxgiAdapter::RegisterVideoMemoryBudgetChangeNotificationEvent(HANDLE hEvent, DWORD* pdwCookie)
{
	return m_idxgiAdapter4_->RegisterVideoMemoryBudgetChangeNotificationEvent(hEvent, pdwCookie);
}

void MyIdxgiAdapter::UnregisterVideoMemoryBudgetChangeNotification(DWORD dwCookie)
{
	return m_idxgiAdapter4_->UnregisterVideoMemoryBudgetChangeNotification(dwCookie);
}

HRESULT MyIdxgiAdapter::GetDesc3(DXGI_ADAPTER_DESC3* pDesc)
{
	return m_idxgiAdapter4_->GetDesc3(pDesc);
}
