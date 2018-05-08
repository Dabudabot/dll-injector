#pragma once
#include "overlay.h"
#include "idxgifactory.h"

class MyIDXGIAdapter : public IDXGIAdapter4
{
	IDXGIAdapter4* m_idxgiAdapter4_ = nullptr;
	MyIDXGIFactory* m_factory_ = nullptr;

public:
	explicit MyIDXGIAdapter(IDXGIAdapter4** ppvObject);
	~MyIDXGIAdapter() = default;
	HRESULT QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG AddRef() override;
	ULONG Release() override;
	HRESULT SetPrivateData(const GUID& Name, UINT DataSize, const void* pData) override;
	HRESULT SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown) override;
	HRESULT GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData) override;
	HRESULT GetParent(const IID& riid, void** ppParent) override;
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

