#pragma once
class MyIDXGIFactory1 : public IDXGIFactory1
{
	void* m_pFactory_ = nullptr;

public:
	MyIDXGIFactory1(void** ppFactory);
	~MyIDXGIFactory1() = default;
	HRESULT QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG AddRef() override;
	ULONG Release() override;
	HRESULT SetPrivateData(const GUID& Name, UINT DataSize, const void* pData) override;
	HRESULT SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown) override;
	HRESULT GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData) override;
	HRESULT GetParent(const IID& riid, void** ppParent) override;
	HRESULT EnumAdapters(UINT Adapter, IDXGIAdapter** ppAdapter) override;
	HRESULT MakeWindowAssociation(HWND WindowHandle, UINT Flags) override;
	HRESULT GetWindowAssociation(HWND* pWindowHandle) override;
	HRESULT CreateSwapChain(IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain) override;
	HRESULT CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter** ppAdapter) override;
	HRESULT EnumAdapters1(UINT Adapter, IDXGIAdapter1** ppAdapter) override;
	BOOL IsCurrent() override;
};

