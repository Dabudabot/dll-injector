// idxgiswapchain.cpp : implementation of swapchain inteface
#include "stdafx.h"
#include "idxgiswapchain.h"

MyIdxgiSwapChain::MyIdxgiSwapChain(IDXGISwapChain** ppSwapChain, Overlay *overlay)
{
	m_ppSwapChain_ = static_cast<IDXGISwapChain4*>(*ppSwapChain);
	m_overlay_ = overlay;
}

MyIdxgiSwapChain::MyIdxgiSwapChain(IDXGISwapChain1** ppSwapChain, Overlay *overlay)
{
	m_ppSwapChain_ = static_cast<IDXGISwapChain4*>(*ppSwapChain);
	m_overlay_ = overlay;
}

HRESULT MyIdxgiSwapChain::QueryInterface(const IID& riid, void ** ppvObject)
{
	return m_ppSwapChain_->QueryInterface(riid, ppvObject);
}

ULONG MyIdxgiSwapChain::AddRef()
{
	return m_ppSwapChain_->AddRef();
}

ULONG MyIdxgiSwapChain::Release()
{
	if (m_overlay_ != nullptr)
	{
		m_overlay_->unloadContent();
		free(m_overlay_);
	}
	return m_ppSwapChain_->Release();
}

HRESULT MyIdxgiSwapChain::SetPrivateData(const GUID & Name, UINT DataSize, const void * pData)
{
	return m_ppSwapChain_->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIdxgiSwapChain::SetPrivateDataInterface(const GUID & Name, const IUnknown * pUnknown)
{
	return m_ppSwapChain_->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIdxgiSwapChain::GetPrivateData(const GUID & Name, UINT * pDataSize, void * pData)
{
	return m_ppSwapChain_->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIdxgiSwapChain::GetParent(const IID & riid, void ** ppParent)
{
	return m_ppSwapChain_->GetParent(riid, ppParent);
}

HRESULT MyIdxgiSwapChain::GetDevice(const IID & riid, void ** ppDevice)
{
	return m_ppSwapChain_->GetDevice(riid, ppDevice);
}

HRESULT MyIdxgiSwapChain::Present(UINT SyncInterval, UINT Flags)
{
	m_overlay_->render();
	return m_ppSwapChain_->Present(SyncInterval, Flags);
}

HRESULT MyIdxgiSwapChain::GetBuffer(UINT Buffer, const IID & riid, void ** ppSurface)
{
	return m_ppSwapChain_->GetBuffer(Buffer, riid, ppSurface);
}

HRESULT MyIdxgiSwapChain::SetFullscreenState(BOOL Fullscreen, IDXGIOutput* pTarget)
{
	return m_ppSwapChain_->SetFullscreenState(Fullscreen, pTarget);
}

HRESULT MyIdxgiSwapChain::GetFullscreenState(BOOL* pFullscreen, IDXGIOutput** ppTarget)
{
	return m_ppSwapChain_->GetFullscreenState(pFullscreen, ppTarget);
}

HRESULT MyIdxgiSwapChain::GetDesc(DXGI_SWAP_CHAIN_DESC* pDesc)
{
	return m_ppSwapChain_->GetDesc(pDesc);
}

HRESULT MyIdxgiSwapChain::ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat,
	UINT SwapChainFlags)
{
	return m_ppSwapChain_->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

HRESULT MyIdxgiSwapChain::ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters)
{
	return m_ppSwapChain_->ResizeTarget(pNewTargetParameters);
}

HRESULT MyIdxgiSwapChain::GetContainingOutput(IDXGIOutput** ppOutput)
{
	return m_ppSwapChain_->GetContainingOutput(ppOutput);
}

HRESULT MyIdxgiSwapChain::GetFrameStatistics(DXGI_FRAME_STATISTICS* pStats)
{
	return m_ppSwapChain_->GetFrameStatistics(pStats);
}

HRESULT MyIdxgiSwapChain::GetLastPresentCount(UINT* pLastPresentCount)
{
	return m_ppSwapChain_->GetLastPresentCount(pLastPresentCount);
}

HRESULT MyIdxgiSwapChain::GetDesc1(DXGI_SWAP_CHAIN_DESC1* pDesc)
{
	return m_ppSwapChain_->GetDesc1(pDesc);
}

HRESULT MyIdxgiSwapChain::GetFullscreenDesc(DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pDesc)
{
	return m_ppSwapChain_->GetFullscreenDesc(pDesc);
}

HRESULT MyIdxgiSwapChain::GetHwnd(HWND* pHwnd)
{
	return m_ppSwapChain_->GetHwnd(pHwnd);
}

HRESULT MyIdxgiSwapChain::GetCoreWindow(const IID& refiid, void** ppUnk)
{
	return m_ppSwapChain_->GetCoreWindow(refiid, ppUnk);
}

HRESULT MyIdxgiSwapChain::Present1(UINT SyncInterval, UINT PresentFlags,
	const DXGI_PRESENT_PARAMETERS* pPresentParameters)
{
	m_overlay_->render();
	return m_ppSwapChain_->Present1(SyncInterval, PresentFlags, pPresentParameters);
}

BOOL MyIdxgiSwapChain::IsTemporaryMonoSupported()
{
	return m_ppSwapChain_->IsTemporaryMonoSupported();
}

HRESULT MyIdxgiSwapChain::GetRestrictToOutput(IDXGIOutput** ppRestrictToOutput)
{
	return m_ppSwapChain_->GetRestrictToOutput(ppRestrictToOutput);
}

HRESULT MyIdxgiSwapChain::SetRotation(DXGI_MODE_ROTATION Rotation)
{
	return m_ppSwapChain_->SetRotation(Rotation);
}

HRESULT MyIdxgiSwapChain::GetRotation(DXGI_MODE_ROTATION* pRotation)
{
	return m_ppSwapChain_->GetRotation(pRotation);
}

HRESULT MyIdxgiSwapChain::SetSourceSize(UINT Width, UINT Height)
{
	return m_ppSwapChain_->SetSourceSize(Width, Height);
}

HRESULT MyIdxgiSwapChain::GetSourceSize(UINT* pWidth, UINT* pHeight)
{
	return m_ppSwapChain_->GetSourceSize(pWidth, pHeight);
}

HRESULT MyIdxgiSwapChain::SetMaximumFrameLatency(UINT MaxLatency)
{
	return m_ppSwapChain_->SetMaximumFrameLatency(MaxLatency);
}

HRESULT MyIdxgiSwapChain::GetMaximumFrameLatency(UINT* pMaxLatency)
{
	return m_ppSwapChain_->GetMaximumFrameLatency(pMaxLatency);
}

HANDLE MyIdxgiSwapChain::GetFrameLatencyWaitableObject()
{
	return m_ppSwapChain_->GetFrameLatencyWaitableObject();
}

HRESULT MyIdxgiSwapChain::SetMatrixTransform(const DXGI_MATRIX_3X2_F* pMatrix)
{
	return m_ppSwapChain_->SetMatrixTransform(pMatrix);
}

HRESULT MyIdxgiSwapChain::GetMatrixTransform(DXGI_MATRIX_3X2_F* pMatrix)
{
	return m_ppSwapChain_->GetMatrixTransform(pMatrix);
}

UINT MyIdxgiSwapChain::GetCurrentBackBufferIndex()
{
	return m_ppSwapChain_->GetCurrentBackBufferIndex();
}

HRESULT MyIdxgiSwapChain::ResizeBuffers1(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT Format,
	UINT SwapChainFlags, const UINT* pCreationNodeMask, IUnknown* const* ppPresentQueue)
{
	return m_ppSwapChain_->ResizeBuffers1(BufferCount, Width, Height, Format,
		SwapChainFlags,  pCreationNodeMask, ppPresentQueue);
}

HRESULT MyIdxgiSwapChain::SetHDRMetaData(DXGI_HDR_METADATA_TYPE Type, UINT Size, void* pMetaData)
{
	return m_ppSwapChain_->SetHDRMetaData(Type, Size, pMetaData);
}

HRESULT MyIdxgiSwapChain::SetBackgroundColor(const DXGI_RGBA* pColor)
{
	return m_ppSwapChain_->SetBackgroundColor(pColor);
}

HRESULT MyIdxgiSwapChain::GetBackgroundColor(DXGI_RGBA* pColor)
{
	return m_ppSwapChain_->GetBackgroundColor(pColor);
}

HRESULT MyIdxgiSwapChain::CheckColorSpaceSupport(DXGI_COLOR_SPACE_TYPE ColorSpace, UINT* pColorSpaceSupport)
{
	return m_ppSwapChain_->CheckColorSpaceSupport(ColorSpace, pColorSpaceSupport);
}

HRESULT MyIdxgiSwapChain::SetColorSpace1(DXGI_COLOR_SPACE_TYPE ColorSpace)
{
	return m_ppSwapChain_->SetColorSpace1(ColorSpace);
}
