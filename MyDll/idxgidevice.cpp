#include "stdafx.h"
#include "idxgidevice.h"

MyIDXGIDevice::MyIDXGIDevice(IDXGIDevice2** ppvObject)
{
	m_pDevice_ = *ppvObject;
}

HRESULT MyIDXGIDevice::QueryInterface(const IID& riid, void** ppvObject)
{
	return m_pDevice_->QueryInterface(riid, ppvObject);
}

ULONG MyIDXGIDevice::AddRef()
{
	return m_pDevice_->AddRef();
}

ULONG MyIDXGIDevice::Release()
{
	free(m_pIdxgiAdapter_);
	return m_pDevice_->Release();
}

HRESULT MyIDXGIDevice::SetPrivateData(const GUID& Name, UINT DataSize, const void* pData)
{
	return m_pDevice_->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIDXGIDevice::SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown)
{
	return m_pDevice_->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIDXGIDevice::GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData)
{
	return m_pDevice_->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIDXGIDevice::GetParent(const IID& riid, void** ppParent)
{
	const auto result = m_pDevice_->GetParent(riid, ppParent);
	if (riid == __uuidof(IDXGIAdapter) ||
		riid == __uuidof(IDXGIAdapter1) ||
		riid == __uuidof(IDXGIAdapter2) ||
		riid == __uuidof(IDXGIAdapter3) ||
		riid == __uuidof(IDXGIAdapter4))
	{
		auto adapter = static_cast<IDXGIAdapter4*>(*ppParent);
		m_pIdxgiAdapter_ = new MyIDXGIAdapter(&adapter);
		ppParent = reinterpret_cast<void**>(&m_pIdxgiAdapter_);
	}
	return result;
}

HRESULT MyIDXGIDevice::GetAdapter(IDXGIAdapter** pAdapter)
{
	return m_pDevice_->GetAdapter(pAdapter);
}

HRESULT MyIDXGIDevice::CreateSurface(const DXGI_SURFACE_DESC* pDesc, UINT NumSurfaces, DXGI_USAGE Usage,
	const DXGI_SHARED_RESOURCE* pSharedResource, IDXGISurface** ppSurface)
{
	return m_pDevice_->CreateSurface(pDesc, NumSurfaces, Usage,
		 pSharedResource, ppSurface);
}

HRESULT MyIDXGIDevice::QueryResourceResidency(IUnknown* const* ppResources, DXGI_RESIDENCY* pResidencyStatus,
	UINT NumResources)
{
	return m_pDevice_->QueryResourceResidency(ppResources, pResidencyStatus, NumResources);
}

HRESULT MyIDXGIDevice::SetGPUThreadPriority(INT Priority)
{
	return m_pDevice_->SetGPUThreadPriority(Priority);
}

HRESULT MyIDXGIDevice::GetGPUThreadPriority(INT* pPriority)
{
	return m_pDevice_->GetGPUThreadPriority(pPriority);
}

HRESULT MyIDXGIDevice::SetMaximumFrameLatency(UINT MaxLatency)
{
	return m_pDevice_->SetMaximumFrameLatency(MaxLatency);
}

HRESULT MyIDXGIDevice::GetMaximumFrameLatency(UINT* pMaxLatency)
{
	return m_pDevice_->GetMaximumFrameLatency(pMaxLatency);
}

HRESULT MyIDXGIDevice::OfferResources(UINT NumResources, IDXGIResource* const* ppResources,
	DXGI_OFFER_RESOURCE_PRIORITY Priority)
{
	return m_pDevice_->OfferResources(NumResources, ppResources, Priority);
}

HRESULT MyIDXGIDevice::ReclaimResources(UINT NumResources, IDXGIResource* const* ppResources, BOOL* pDiscarded)
{
	return m_pDevice_->ReclaimResources(NumResources, ppResources, pDiscarded);
}

HRESULT MyIDXGIDevice::EnqueueSetEvent(HANDLE hEvent)
{
	return m_pDevice_->EnqueueSetEvent(hEvent);
}
