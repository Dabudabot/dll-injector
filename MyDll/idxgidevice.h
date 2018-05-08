#pragma once
#include "overlay.h"
#include "idxgiadapter.h"

class MyIDXGIDevice : public IDXGIDevice2
{
	IDXGIDevice2* m_pDevice_ = nullptr;
	MyIDXGIAdapter* m_pIdxgiAdapter_ = nullptr;

public:
	explicit MyIDXGIDevice(IDXGIDevice2** ppvObject);
	~MyIDXGIDevice() = default;
	HRESULT QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG AddRef() override;
	ULONG Release() override;
	HRESULT SetPrivateData(const GUID& Name, UINT DataSize, const void* pData) override;
	HRESULT SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown) override;
	HRESULT GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData) override;
	HRESULT GetParent(const IID& riid, void** ppParent) override;
	HRESULT GetAdapter(IDXGIAdapter** pAdapter) override;
	HRESULT CreateSurface(const DXGI_SURFACE_DESC* pDesc, UINT NumSurfaces, DXGI_USAGE Usage,
		const DXGI_SHARED_RESOURCE* pSharedResource, IDXGISurface** ppSurface) override;
	HRESULT QueryResourceResidency(IUnknown* const* ppResources, DXGI_RESIDENCY* pResidencyStatus, UINT NumResources)
	override;
	HRESULT SetGPUThreadPriority(INT Priority) override;
	HRESULT GetGPUThreadPriority(INT* pPriority) override;
	HRESULT SetMaximumFrameLatency(UINT MaxLatency) override;
	HRESULT GetMaximumFrameLatency(UINT* pMaxLatency) override;
	HRESULT OfferResources(UINT NumResources, IDXGIResource* const* ppResources, DXGI_OFFER_RESOURCE_PRIORITY Priority)
	override;
	HRESULT ReclaimResources(UINT NumResources, IDXGIResource* const* ppResources, BOOL* pDiscarded) override;
	HRESULT EnqueueSetEvent(HANDLE hEvent) override;
};

