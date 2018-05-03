#include "stdafx.h"
#include "idxgiswapchain.h"

MyIdxgiSwapChain::MyIdxgiSwapChain(IDXGISwapChain** ppSwapChain, Overlay *overlay)
{
	m_ppSwapChain_ = *ppSwapChain;
	m_overlay_ = overlay;
}

HRESULT MyIdxgiSwapChain::QueryInterface(REFIID riid, void ** ppvObject)
{
	return m_ppSwapChain_->QueryInterface(riid, ppvObject);
}

ULONG MyIdxgiSwapChain::AddRef()
{
	return m_ppSwapChain_->AddRef();
}

ULONG MyIdxgiSwapChain::Release()
{
	m_overlay_->unloadContent();
	return m_ppSwapChain_->Release();
}

HRESULT MyIdxgiSwapChain::SetPrivateData(REFGUID Name, UINT DataSize, const void * pData)
{
	return m_ppSwapChain_->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIdxgiSwapChain::SetPrivateDataInterface(REFGUID Name, const IUnknown * pUnknown)
{
	return m_ppSwapChain_->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIdxgiSwapChain::GetPrivateData(REFGUID Name, UINT * pDataSize, void * pData)
{
	return m_ppSwapChain_->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIdxgiSwapChain::GetParent(REFIID riid, void ** ppParent)
{
	return m_ppSwapChain_->GetParent(riid, ppParent);
}

HRESULT MyIdxgiSwapChain::GetDevice(REFIID riid, void ** ppDevice)
{
	return m_ppSwapChain_->GetDevice(riid, ppDevice);
}

HRESULT MyIdxgiSwapChain::Present(UINT SyncInterval, UINT Flags)
{
	m_overlay_->render();
	free(m_overlay_);
	//MessageBox(nullptr, L"Present function called", L"MyDll.dll", MB_OK);
	return m_ppSwapChain_->Present(SyncInterval, Flags);
}

HRESULT MyIdxgiSwapChain::GetBuffer(UINT Buffer, REFIID riid, void ** ppSurface)
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
