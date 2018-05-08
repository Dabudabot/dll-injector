// idxgidevice.h : definitions of the class MyIDXGIDevice

#pragma once

class MyIdxgiAdapter;	// in order not to create cyclamatic inclidings

/**
 * \brief inherits from IDXGIDevice2
 * realisation of wrapper pattern, it just wraps the original functions of the IDXGIDevice2
 * changes the logic only of GetParent and Release functions
 * needed only if swapchain will be achived throught quering interfaces
 */
class MyIdxgiDevice : public IDXGIDevice2  // NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
{
	IDXGIDevice2* m_pDevice_ = nullptr;			//original device
	MyIdxgiAdapter* m_pIdxgiAdapter_ = nullptr;	//in case of GetParent we will give it to caller

	//ctors and dtors
public:
	explicit MyIdxgiDevice(IDXGIDevice2** ppvObject);
	virtual ~MyIdxgiDevice() = default;

	// original functions to be wrapper with enhancments
public:
	/**
	* \brief releases memory
	* \return original function call return
	*/
	ULONG Release() override;
	/**
	* \brief additional logic is replacing original ppParent in case of
	* IDXGIAdapter, IDXGIAdapter1, IDXGIAdapter2, IDXGIAdapter3, IDXGIAdapter4
	* \param riid identificator of inteface
	* \param ppParent returning object stored here
	* \return result of the function
	*/
	HRESULT GetParent(const IID& riid, void** ppParent) override;

	// original functions to be wrapper without changing the logic
public:
	HRESULT QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG AddRef() override;
	HRESULT SetPrivateData(const GUID& Name, UINT DataSize, const void* pData) override;
	HRESULT SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown) override;
	HRESULT GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData) override;
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

