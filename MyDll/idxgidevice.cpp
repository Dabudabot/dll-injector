#include "stdafx.h"
#include "idxgidevice.h"

MyIdxgiDevice::MyIdxgiDevice(void** ppvObject)
{
	m_pDevice_ = static_cast<IDXGIDevice2*>(*ppvObject);
}

HRESULT MyIdxgiDevice::QueryInterface(const IID& riid, void** ppvObject)
{
	return m_pDevice_->QueryInterface(riid, ppvObject);
}

ULONG MyIdxgiDevice::AddRef()
{
	return m_pDevice_->AddRef();
}

ULONG MyIdxgiDevice::Release()
{
	if (m_pIdxgiAdapter_ != nullptr)
	{
		m_pIdxgiAdapter_->Release();
		free(m_pIdxgiAdapter_);
	}
	return m_pDevice_->Release();
}

HRESULT MyIdxgiDevice::SetPrivateData(const GUID& Name, UINT DataSize, const void* pData)
{
	return m_pDevice_->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIdxgiDevice::SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown)
{
	return m_pDevice_->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIdxgiDevice::GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData)
{
	return m_pDevice_->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIdxgiDevice::GetParent(const IID& riid, void** ppParent)
{
	wchar_t text[100];
	wsprintf(text, L"MyIdxgiDevice::GetParent function called with %d, IDXGIAdapter = %d, IDXGIAdapter1 = %d, IDXGIAdapter2 = %d, IDXGIAdapter3 = %d, IDXGIAdapter4 = %d",
		riid, __uuidof(IDXGIAdapter), __uuidof(IDXGIAdapter1), __uuidof(IDXGIAdapter2), __uuidof(IDXGIAdapter3), __uuidof(IDXGIAdapter4));
	MessageBox(nullptr, text, L"MyDll.dll", MB_OK);

	const auto result = m_pDevice_->GetParent(riid, ppParent);
	if (riid == __uuidof(IDXGIAdapter) ||
		riid == __uuidof(IDXGIAdapter1) ||
		riid == __uuidof(IDXGIAdapter2) ||
		riid == __uuidof(IDXGIAdapter3) ||
		riid == __uuidof(IDXGIAdapter4))
	{
		auto adapter = static_cast<IDXGIAdapter4*>(*ppParent);
		m_pIdxgiAdapter_ = new MyIdxgiAdapter(&adapter);
		auto a = static_cast<void*>(m_pIdxgiAdapter_);
		ppParent = &a;
		//ppParent = reinterpret_cast<void**>(&m_pIdxgiAdapter_);
	}
	return result;
}

HRESULT MyIdxgiDevice::GetAdapter(IDXGIAdapter** pAdapter)
{
	return m_pDevice_->GetAdapter(pAdapter);
}

HRESULT MyIdxgiDevice::CreateSurface(const DXGI_SURFACE_DESC* pDesc, UINT NumSurfaces, DXGI_USAGE Usage,
	const DXGI_SHARED_RESOURCE* pSharedResource, IDXGISurface** ppSurface)
{
	return m_pDevice_->CreateSurface(pDesc, NumSurfaces, Usage,
		 pSharedResource, ppSurface);
}

HRESULT MyIdxgiDevice::QueryResourceResidency(IUnknown* const* ppResources, DXGI_RESIDENCY* pResidencyStatus,
	UINT NumResources)
{
	return m_pDevice_->QueryResourceResidency(ppResources, pResidencyStatus, NumResources);
}

HRESULT MyIdxgiDevice::SetGPUThreadPriority(INT Priority)
{
	return m_pDevice_->SetGPUThreadPriority(Priority);
}

HRESULT MyIdxgiDevice::GetGPUThreadPriority(INT* pPriority)
{
	return m_pDevice_->GetGPUThreadPriority(pPriority);
}

HRESULT MyIdxgiDevice::SetMaximumFrameLatency(UINT MaxLatency)
{
	return m_pDevice_->SetMaximumFrameLatency(MaxLatency);
}

HRESULT MyIdxgiDevice::GetMaximumFrameLatency(UINT* pMaxLatency)
{
	return m_pDevice_->GetMaximumFrameLatency(pMaxLatency);
}

HRESULT MyIdxgiDevice::OfferResources(UINT NumResources, IDXGIResource* const* ppResources,
	DXGI_OFFER_RESOURCE_PRIORITY Priority)
{
	return m_pDevice_->OfferResources(NumResources, ppResources, Priority);
}

HRESULT MyIdxgiDevice::ReclaimResources(UINT NumResources, IDXGIResource* const* ppResources, BOOL* pDiscarded)
{
	return m_pDevice_->ReclaimResources(NumResources, ppResources, pDiscarded);
}

HRESULT MyIdxgiDevice::EnqueueSetEvent(HANDLE hEvent)
{
	return m_pDevice_->EnqueueSetEvent(hEvent);
}
