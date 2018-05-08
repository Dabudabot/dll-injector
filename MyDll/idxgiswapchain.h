// My replacement of the swapchain inteface
#pragma once

#include "overlay.h"

class MyIdxgiSwapChain : public IDXGISwapChain4
{
	IDXGISwapChain4* m_ppSwapChain_ = nullptr;
	Overlay* m_overlay_ = nullptr;

public:
	~MyIdxgiSwapChain() = default;
	/**
	 * \brief ctor to init original swapchain and overlay
	 * \param ppSwapChain original one
	 * \param overlay to draw smt on
	 */
	explicit MyIdxgiSwapChain(IDXGISwapChain** ppSwapChain, Overlay *overlay);
	explicit MyIdxgiSwapChain(IDXGISwapChain1** ppSwapChain, Overlay *overlay);

	// Original functions
public:
	HRESULT QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG AddRef() override;
	ULONG Release() override;
	HRESULT SetPrivateData(const GUID& Name, UINT DataSize, const void* pData) override;
	HRESULT SetPrivateDataInterface(const GUID& Name, const IUnknown* pUnknown) override;
	HRESULT GetPrivateData(const GUID& Name, UINT* pDataSize, void* pData) override;
	HRESULT GetParent(const IID& riid, void** ppParent) override;
	HRESULT GetDevice(const IID& riid, void** ppDevice) override;
	HRESULT Present(UINT SyncInterval, UINT Flags) override;
	HRESULT GetBuffer(UINT Buffer, const IID& riid, void** ppSurface) override;
	HRESULT SetFullscreenState(BOOL Fullscreen, IDXGIOutput* pTarget) override;
	HRESULT GetFullscreenState(BOOL* pFullscreen, IDXGIOutput** ppTarget) override;
	HRESULT GetDesc(DXGI_SWAP_CHAIN_DESC* pDesc) override;
	HRESULT ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) override;
	HRESULT ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters) override;
	HRESULT GetContainingOutput(IDXGIOutput** ppOutput) override;
	HRESULT GetFrameStatistics(DXGI_FRAME_STATISTICS* pStats) override;
	HRESULT GetLastPresentCount(UINT* pLastPresentCount) override;
	HRESULT GetDesc1(DXGI_SWAP_CHAIN_DESC1* pDesc) override;
	HRESULT GetFullscreenDesc(DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pDesc) override;
	HRESULT GetHwnd(HWND* pHwnd) override;
	HRESULT GetCoreWindow(const IID& refiid, void** ppUnk) override;
	HRESULT Present1(UINT SyncInterval, UINT PresentFlags, const DXGI_PRESENT_PARAMETERS* pPresentParameters) override;
	BOOL IsTemporaryMonoSupported() override;
	HRESULT GetRestrictToOutput(IDXGIOutput** ppRestrictToOutput) override;
	HRESULT SetRotation(DXGI_MODE_ROTATION Rotation) override;
	HRESULT GetRotation(DXGI_MODE_ROTATION* pRotation) override;
	HRESULT SetSourceSize(UINT Width, UINT Height) override;
	HRESULT GetSourceSize(UINT* pWidth, UINT* pHeight) override;
	HRESULT SetMaximumFrameLatency(UINT MaxLatency) override;
	HRESULT GetMaximumFrameLatency(UINT* pMaxLatency) override;
	HANDLE GetFrameLatencyWaitableObject() override;
	HRESULT SetMatrixTransform(const DXGI_MATRIX_3X2_F* pMatrix) override;
	HRESULT GetMatrixTransform(DXGI_MATRIX_3X2_F* pMatrix) override;
	UINT GetCurrentBackBufferIndex() override;
	HRESULT ResizeBuffers1(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT Format, UINT SwapChainFlags,
		const UINT* pCreationNodeMask, IUnknown* const* ppPresentQueue) override;
	HRESULT SetHDRMetaData(DXGI_HDR_METADATA_TYPE Type, UINT Size, void* pMetaData) override;
	HRESULT SetBackgroundColor(const DXGI_RGBA* pColor) override;
	HRESULT GetBackgroundColor(DXGI_RGBA* pColor) override;
	HRESULT CheckColorSpaceSupport(DXGI_COLOR_SPACE_TYPE ColorSpace, UINT* pColorSpaceSupport) override;
	HRESULT SetColorSpace1(DXGI_COLOR_SPACE_TYPE ColorSpace) override;
};
