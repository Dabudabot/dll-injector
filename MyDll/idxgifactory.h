// idxgidevice.h : definitions of the class MyIDXGIDevice

#pragma once
#include "idxgiswapchain.h"
#include "id3d11device.h"

/**
 * \brief 
 */
class MyIdxgiFactory : public IDXGIFactory2  // NOLINT(cppcoreguidelines-special-member-functions, hicpp-special-member-functions)
{
	IDXGIFactory2* m_pFactory_ = nullptr;

	//ctors and dtors
public:
	explicit MyIdxgiFactory(void** ppFactory);
	virtual ~MyIdxgiFactory() = default;

	// original functions to be wrapper with enhancments
public:
	HRESULT CreateSwapChain(IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain) override;
	HRESULT CreateSwapChainForHwnd(IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc,
		const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput,
		IDXGISwapChain1** ppSwapChain) override;
	HRESULT CreateSwapChainForCoreWindow(IUnknown* pDevice, IUnknown* pWindow, const DXGI_SWAP_CHAIN_DESC1* pDesc,
		IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) override;
	HRESULT CreateSwapChainForComposition(IUnknown* pDevice, const DXGI_SWAP_CHAIN_DESC1* pDesc,
		IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) override;

	// original functions to be wrapper without changing the logic
public:
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
	HRESULT CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter** ppAdapter) override;
	HRESULT EnumAdapters1(UINT Adapter, IDXGIAdapter1** ppAdapter) override;
	BOOL IsCurrent() override;
	BOOL IsWindowedStereoEnabled() override;
	HRESULT GetSharedResourceAdapterLuid(HANDLE hResource, LUID* pLuid) override;
	HRESULT RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg, DWORD* pdwCookie) override;
	HRESULT RegisterStereoStatusEvent(HANDLE hEvent, DWORD* pdwCookie) override;
	void UnregisterStereoStatus(DWORD dwCookie) override;
	HRESULT RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg, DWORD* pdwCookie) override;
	HRESULT RegisterOcclusionStatusEvent(HANDLE hEvent, DWORD* pdwCookie) override;
	void UnregisterOcclusionStatus(DWORD dwCookie) override;
};

