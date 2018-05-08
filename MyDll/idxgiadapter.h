// idxgiadapter.h : definitions of the class MyIDXGIAdapter

#pragma once

class MyIdxgiFactory; // in order not to create cyclamatic inclidings

/**
 * \brief inherits from IDXGIAdapter4
 * realisation of wrapper pattern, it just wraps the original functions of the IDXGIAdapter4
 * changes the logic only of GetParent and Release functions
 * needed only if swapchain will be achived throught quering interfaces
 */
class MyIdxgiAdapter : public IDXGIAdapter4  // NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
{
	IDXGIAdapter4 * m_idxgiAdapter4_ = nullptr;	//original adapter
	MyIdxgiFactory* m_factory_ = nullptr;		//in case of GetParent we will give it to caller
	//ctors and dtors
public:
	explicit MyIdxgiAdapter(IDXGIAdapter4** ppvObject);
	virtual ~MyIdxgiAdapter() = default;

	// original functions to be wrapper with enhancments
public:
	/**
	* \brief releases memory
	* \return original function call return
	*/
	ULONG Release() override;
	/**
	 * \brief additional logic is replacing original ppParent in case of 
	 * IDXGIFactory, IDXGIFactory1, IDXGIFactory2
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
	HRESULT EnumOutputs(UINT Output, IDXGIOutput** ppOutput) override;
	HRESULT GetDesc(DXGI_ADAPTER_DESC* pDesc) override;
	HRESULT CheckInterfaceSupport(const GUID& InterfaceName, LARGE_INTEGER* pUMDVersion) override;
	HRESULT GetDesc1(DXGI_ADAPTER_DESC1* pDesc) override;
	HRESULT GetDesc2(DXGI_ADAPTER_DESC2* pDesc) override;
	HRESULT RegisterHardwareContentProtectionTeardownStatusEvent(HANDLE hEvent, DWORD* pdwCookie) override;
	void UnregisterHardwareContentProtectionTeardownStatus(DWORD dwCookie) override;
	HRESULT QueryVideoMemoryInfo(UINT NodeIndex, DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup,
		DXGI_QUERY_VIDEO_MEMORY_INFO* pVideoMemoryInfo) override;
	HRESULT SetVideoMemoryReservation(UINT NodeIndex, DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup, UINT64 Reservation)
	override;
	HRESULT RegisterVideoMemoryBudgetChangeNotificationEvent(HANDLE hEvent, DWORD* pdwCookie) override;
	void UnregisterVideoMemoryBudgetChangeNotification(DWORD dwCookie) override;
	HRESULT GetDesc3(DXGI_ADAPTER_DESC3* pDesc) override;
};

