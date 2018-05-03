#include "stdafx.h"
#include "idxgiswapchain.h"

MyIdxgiSwapChain::MyIdxgiSwapChain(IDXGISwapChain** ppSwapChain)
{
	m_ppSwapChain = *ppSwapChain;
}

HRESULT MyIdxgiSwapChain::QueryInterface(REFIID riid, void ** ppvObject)
{
	return m_ppSwapChain->QueryInterface(riid, ppvObject);
}

ULONG MyIdxgiSwapChain::AddRef()
{
	return m_ppSwapChain->AddRef();
}

ULONG MyIdxgiSwapChain::Release()
{
	return m_ppSwapChain->Release();
}

HRESULT MyIdxgiSwapChain::SetPrivateData(REFGUID Name, UINT DataSize, const void * pData)
{
	return m_ppSwapChain->SetPrivateData(Name, DataSize, pData);
}

HRESULT MyIdxgiSwapChain::SetPrivateDataInterface(REFGUID Name, const IUnknown * pUnknown)
{
	return m_ppSwapChain->SetPrivateDataInterface(Name, pUnknown);
}

HRESULT MyIdxgiSwapChain::GetPrivateData(REFGUID Name, UINT * pDataSize, void * pData)
{
	return m_ppSwapChain->GetPrivateData(Name, pDataSize, pData);
}

HRESULT MyIdxgiSwapChain::GetParent(REFIID riid, void ** ppParent)
{
	return m_ppSwapChain->GetParent(riid, ppParent);
}

HRESULT MyIdxgiSwapChain::GetDevice(REFIID riid, void ** ppDevice)
{
	return m_ppSwapChain->GetDevice(riid, ppDevice);
}

HRESULT MyIdxgiSwapChain::Present(UINT SyncInterval, UINT Flags)
{
	MessageBox(nullptr, L"Present function called", L"MyDll.dll", MB_OK);
	return m_ppSwapChain->Present(SyncInterval, Flags);
}

HRESULT MyIdxgiSwapChain::GetBuffer(UINT Buffer, REFIID riid, void ** ppSurface)
{
	return m_ppSwapChain->GetBuffer(Buffer, riid, ppSurface);
}

HRESULT MyIdxgiSwapChain::SetFullscreenState(BOOL Fullscreen, IDXGIOutput* pTarget)
{
	return m_ppSwapChain->SetFullscreenState(Fullscreen, pTarget);
}

HRESULT MyIdxgiSwapChain::GetFullscreenState(BOOL* pFullscreen, IDXGIOutput** ppTarget)
{
	return m_ppSwapChain->GetFullscreenState(pFullscreen, ppTarget);
}

HRESULT MyIdxgiSwapChain::GetDesc(DXGI_SWAP_CHAIN_DESC* pDesc)
{
	return m_ppSwapChain->GetDesc(pDesc);
}

HRESULT MyIdxgiSwapChain::ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat,
	UINT SwapChainFlags)
{
	return m_ppSwapChain->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

HRESULT MyIdxgiSwapChain::ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters)
{
	return m_ppSwapChain->ResizeTarget(pNewTargetParameters);
}

HRESULT MyIdxgiSwapChain::GetContainingOutput(IDXGIOutput** ppOutput)
{
	return m_ppSwapChain->GetContainingOutput(ppOutput);
}

HRESULT MyIdxgiSwapChain::GetFrameStatistics(DXGI_FRAME_STATISTICS* pStats)
{
	return m_ppSwapChain->GetFrameStatistics(pStats);
}

HRESULT MyIdxgiSwapChain::GetLastPresentCount(UINT* pLastPresentCount)
{
	return m_ppSwapChain->GetLastPresentCount(pLastPresentCount);
}
