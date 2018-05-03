#pragma once

#include <DXGI.h>

class MyIdxgiSwapChain : public IDXGISwapChain
{
public:
	IDXGISwapChain* m_ppSwapChain = nullptr;

public:
	explicit MyIdxgiSwapChain(IDXGISwapChain** ppSwapChain);

	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) override;

	ULONG STDMETHODCALLTYPE AddRef(void) override;

	ULONG STDMETHODCALLTYPE Release(void) override;

	HRESULT STDMETHODCALLTYPE SetPrivateData(
		/* [annotation][in] */
		__in  REFGUID Name,
		/* [in] */ UINT DataSize,
		/* [annotation][in] */
		__in_bcount(DataSize)  const void *pData) override;

	HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
		/* [annotation][in] */
		__in  REFGUID Name,
		/* [annotation][in] */
		__in  const IUnknown *pUnknown) override;

	HRESULT STDMETHODCALLTYPE GetPrivateData(
		/* [annotation][in] */
		__in  REFGUID Name,
		/* [annotation][out][in] */
		__inout  UINT *pDataSize,
		/* [annotation][out] */
		__out_bcount(*pDataSize)  void *pData) override;

	HRESULT STDMETHODCALLTYPE GetParent(
		/* [annotation][in] */
		__in  REFIID riid,
		/* [annotation][retval][out] */
		__out  void **ppParent) override;

	HRESULT STDMETHODCALLTYPE GetDevice(
		/* [annotation][in] */
		__in  REFIID riid,
		/* [annotation][retval][out] */
		__out  void **ppDevice) override;

	HRESULT STDMETHODCALLTYPE Present(
		/* [in] */ UINT SyncInterval,
		/* [in] */ UINT Flags) override;

	HRESULT STDMETHODCALLTYPE GetBuffer(
		/* [in] */ UINT Buffer,
		/* [annotation][in] */
		__in  REFIID riid,
		/* [annotation][out][in] */
		__out  void **ppSurface) override;

	HRESULT STDMETHODCALLTYPE SetFullscreenState(
		/* [in] */ BOOL Fullscreen,
		/* [annotation][in] */
		__in_opt  IDXGIOutput *pTarget) override;

	HRESULT STDMETHODCALLTYPE GetFullscreenState(
		/* [annotation][out] */
		__out  BOOL *pFullscreen,
		/* [annotation][out] */
		__out  IDXGIOutput **ppTarget) override;

	HRESULT STDMETHODCALLTYPE GetDesc(
		/* [annotation][out] */
		__out  DXGI_SWAP_CHAIN_DESC *pDesc) override;

	HRESULT STDMETHODCALLTYPE ResizeBuffers(
		/* [in] */ UINT BufferCount,
		/* [in] */ UINT Width,
		/* [in] */ UINT Height,
		/* [in] */ DXGI_FORMAT NewFormat,
		/* [in] */ UINT SwapChainFlags) override;

	HRESULT STDMETHODCALLTYPE ResizeTarget(
		/* [annotation][in] */
		__in  const DXGI_MODE_DESC *pNewTargetParameters) override;

	HRESULT STDMETHODCALLTYPE GetContainingOutput(
		/* [annotation][out] */
		__out  IDXGIOutput **ppOutput) override;

	HRESULT STDMETHODCALLTYPE GetFrameStatistics(
		/* [annotation][out] */
		__out  DXGI_FRAME_STATISTICS *pStats) override;

	HRESULT STDMETHODCALLTYPE GetLastPresentCount(
		/* [annotation][out] */
		__out  UINT *pLastPresentCount) override;
};