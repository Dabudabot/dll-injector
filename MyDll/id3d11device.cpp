#include "stdafx.h"
#include "id3d11device.h"

MyId3D11Device::MyId3D11Device(ID3D11Device** ppDevice, Overlay *overlay)
{
	m_pDevice_ = static_cast<ID3D11Device5*>(*ppDevice);
	m_overlay = overlay;
}

HRESULT MyId3D11Device::QueryInterface(const IID& riid, void** ppvObject)
{
	/*
	wchar_t text[100];
	wsprintf(text, L"MyId3D11Device::QueryInterface function called with %d, ID3D11Device1 = %d, ID3D11Device2 = %d, ID3D11Device3 = %d, ID3D11Device4 = %d, ID3D11Device5 = %d, IDXGIDevice = %d, IDXGIDevice1 = %d, IDXGIDevice2 = %d", 
		riid, __uuidof(ID3D11Device1), __uuidof(ID3D11Device2), __uuidof(ID3D11Device3), __uuidof(ID3D11Device4), __uuidof(ID3D11Device5)
		, __uuidof(IDXGIDevice), __uuidof(IDXGIDevice1), __uuidof(IDXGIDevice2));
	MessageBox(nullptr, text, L"MyDll.dll", MB_OK);
	*/

	const auto result = m_pDevice_->QueryInterface(riid, ppvObject);

	if (riid == __uuidof(ID3D11Device1) || 
		riid == __uuidof(ID3D11Device2) ||
		riid == __uuidof(ID3D11Device3) ||
		riid == __uuidof(ID3D11Device4) ||
		riid == __uuidof(ID3D11Device5))
	{
		auto a = static_cast<void*>(m_pDevice_);
		ppvObject = &a;
		//ppvObject = reinterpret_cast<void**>(&m_pDevice);
		MessageBox(nullptr, L"ID3D11Device1", L"MyDll.dll", MB_OK);
	}
	else if (riid == __uuidof(IDXGIDevice)	||
			 riid == __uuidof(IDXGIDevice1)	||
			 riid == __uuidof(IDXGIDevice2))
	{
		m_pIdxgiDevice_ = new MyIdxgiDevice(ppvObject);
		auto a = static_cast<void*>(m_pIdxgiDevice_);
		ppvObject = &a;
		//ppvObject = reinterpret_cast<void**>(&m_pIdxgiDevice);
		//MessageBox(nullptr, L"IDXGIDevice", L"MyDll.dll", MB_OK);
	}

	return result;
}

ULONG MyId3D11Device::AddRef()
{
	return m_pDevice_->AddRef();
}

ULONG MyId3D11Device::Release()
{
	//MessageBox(nullptr, L"MyId3D11Device::Release", L"MyDll.dll", MB_OK);
	//free(m_pIdxgiDevice_);
	if (m_pIdxgiDevice_ != nullptr)
	{
		m_pIdxgiDevice_->Release();
		//free(m_pIdxgiDevice_);
	}

	if (m_pDevice_ == nullptr)
	{
		MessageBox(nullptr, L"MyId3D11Device::Release", L"MyDll.dll", MB_OK);
	}
	//free(m_pIdxgiDevice_);
	return m_pDevice_->Release();
}

HRESULT MyId3D11Device::CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
	ID3D11Buffer** ppBuffer)
{
	return m_pDevice_->CreateBuffer(pDesc, pInitialData, ppBuffer);
}

HRESULT MyId3D11Device::CreateTexture1D(const D3D11_TEXTURE1D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
	ID3D11Texture1D** ppTexture1D)
{
	return m_pDevice_->CreateTexture1D(pDesc, pInitialData, ppTexture1D);
}

HRESULT MyId3D11Device::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
	ID3D11Texture2D** ppTexture2D)
{
	return m_pDevice_->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
}

HRESULT MyId3D11Device::CreateTexture3D(const D3D11_TEXTURE3D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
	ID3D11Texture3D** ppTexture3D)
{
	return m_pDevice_->CreateTexture3D(pDesc, pInitialData, ppTexture3D);
}

HRESULT MyId3D11Device::CreateShaderResourceView(ID3D11Resource* pResource,
	const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
{
	return m_pDevice_->CreateShaderResourceView(pResource, pDesc, ppSRView);
}

HRESULT MyId3D11Device::CreateUnorderedAccessView(ID3D11Resource* pResource,
	const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
{
	return m_pDevice_->CreateUnorderedAccessView(pResource, pDesc, ppUAView);
}

HRESULT MyId3D11Device::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
	ID3D11RenderTargetView** ppRTView)
{
	return m_pDevice_->CreateRenderTargetView(pResource, pDesc, ppRTView);
}

HRESULT MyId3D11Device::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
	ID3D11DepthStencilView** ppDepthStencilView)
{
	return m_pDevice_->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
}

HRESULT MyId3D11Device::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements,
	const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
{
	return m_pDevice_->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

HRESULT MyId3D11Device::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
{
	return m_pDevice_->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

HRESULT MyId3D11Device::CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
	return m_pDevice_->CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
}

HRESULT MyId3D11Device::CreateGeometryShaderWithStreamOutput(const void* pShaderBytecode, SIZE_T BytecodeLength,
	const D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, const UINT* pBufferStrides, UINT NumStrides,
	UINT RasterizedStream, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
	return m_pDevice_->CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength,
		pSODeclaration,  NumEntries,  pBufferStrides,  NumStrides,
		RasterizedStream, pClassLinkage, ppGeometryShader);
}

HRESULT MyId3D11Device::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
	return m_pDevice_->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

HRESULT MyId3D11Device::CreateHullShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11HullShader** ppHullShader)
{
	return m_pDevice_->CreateHullShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
}

HRESULT MyId3D11Device::CreateDomainShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11DomainShader** ppDomainShader)
{
	return m_pDevice_->CreateDomainShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
}

HRESULT MyId3D11Device::CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader)
{
	return m_pDevice_->CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
}

HRESULT MyId3D11Device::CreateClassLinkage(ID3D11ClassLinkage** ppLinkage)
{
	return m_pDevice_->CreateClassLinkage(ppLinkage);
}

HRESULT MyId3D11Device::CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState)
{
	return m_pDevice_->CreateBlendState(pBlendStateDesc, ppBlendState);
}

HRESULT MyId3D11Device::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc,
	ID3D11DepthStencilState** ppDepthStencilState)
{
	return m_pDevice_->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
}

HRESULT MyId3D11Device::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc,
	ID3D11RasterizerState** ppRasterizerState)
{
	return m_pDevice_->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
}

HRESULT MyId3D11Device::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
	return m_pDevice_->CreateSamplerState(pSamplerDesc, ppSamplerState);
}

HRESULT MyId3D11Device::CreateQuery(const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery)
{
	return m_pDevice_->CreateQuery(pQueryDesc, ppQuery);
}

HRESULT MyId3D11Device::CreatePredicate(const D3D11_QUERY_DESC* pPredicateDesc, ID3D11Predicate** ppPredicate)
{
	return m_pDevice_->CreatePredicate(pPredicateDesc, ppPredicate);
}

HRESULT MyId3D11Device::CreateCounter(const D3D11_COUNTER_DESC* pCounterDesc, ID3D11Counter** ppCounter)
{
	return m_pDevice_->CreateCounter(pCounterDesc, ppCounter);
}

HRESULT MyId3D11Device::CreateDeferredContext(UINT ContextFlags, ID3D11DeviceContext** ppDeferredContext)
{
	return m_pDevice_->CreateDeferredContext(ContextFlags, ppDeferredContext);
}

HRESULT MyId3D11Device::OpenSharedResource(HANDLE hResource, const IID& ReturnedInterface, void** ppResource)
{
	return m_pDevice_->OpenSharedResource(hResource, ReturnedInterface, ppResource);
}

HRESULT MyId3D11Device::CheckFormatSupport(DXGI_FORMAT Format, UINT* pFormatSupport)
{
	return m_pDevice_->CheckFormatSupport(Format, pFormatSupport);
}

HRESULT MyId3D11Device::CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels)
{
	return m_pDevice_->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
}

void MyId3D11Device::CheckCounterInfo(D3D11_COUNTER_INFO* pCounterInfo)
{
	m_pDevice_->CheckCounterInfo(pCounterInfo);
}

HRESULT MyId3D11Device::CheckCounter(const D3D11_COUNTER_DESC* pDesc, D3D11_COUNTER_TYPE* pType, UINT* pActiveCounters,
	LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength)
{
	return m_pDevice_->CheckCounter(pDesc, pType,  pActiveCounters,
		szName,  pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
}

HRESULT MyId3D11Device::CheckFeatureSupport(D3D11_FEATURE Feature, void* pFeatureSupportData,
	UINT FeatureSupportDataSize)
{
	return m_pDevice_->CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize);
}

HRESULT MyId3D11Device::GetPrivateData(const GUID& guid, UINT* pDataSize, void* pData)
{
	return m_pDevice_->GetPrivateData(guid, pDataSize, pData);
}

HRESULT MyId3D11Device::SetPrivateData(const GUID& guid, UINT DataSize, const void* pData)
{
	return m_pDevice_->SetPrivateData(guid, DataSize, pData);
}

HRESULT MyId3D11Device::SetPrivateDataInterface(const GUID& guid, const IUnknown* pData)
{
	return m_pDevice_->SetPrivateDataInterface(guid, pData);
}

D3D_FEATURE_LEVEL MyId3D11Device::GetFeatureLevel()
{
	return m_pDevice_->GetFeatureLevel();
}

UINT MyId3D11Device::GetCreationFlags()
{
	return m_pDevice_->GetCreationFlags();
}

HRESULT MyId3D11Device::GetDeviceRemovedReason()
{
	return m_pDevice_->GetDeviceRemovedReason();
}

void MyId3D11Device::GetImmediateContext(ID3D11DeviceContext** ppImmediateContext)
{
	m_pDevice_->GetImmediateContext(ppImmediateContext);
}

HRESULT MyId3D11Device::SetExceptionMode(UINT RaiseFlags)
{
	return m_pDevice_->SetExceptionMode(RaiseFlags);
}

UINT MyId3D11Device::GetExceptionMode()
{
	return m_pDevice_->GetExceptionMode();
}

void MyId3D11Device::GetImmediateContext1(ID3D11DeviceContext1** ppImmediateContext)
{
	return m_pDevice_->GetImmediateContext1(ppImmediateContext);
}

HRESULT MyId3D11Device::CreateDeferredContext1(UINT ContextFlags, ID3D11DeviceContext1** ppDeferredContext)
{
	return m_pDevice_->CreateDeferredContext1(ContextFlags, ppDeferredContext);
}

HRESULT MyId3D11Device::CreateBlendState1(const D3D11_BLEND_DESC1* pBlendStateDesc, ID3D11BlendState1** ppBlendState)
{
	return m_pDevice_->CreateBlendState1(pBlendStateDesc, ppBlendState);
}

HRESULT MyId3D11Device::CreateRasterizerState1(const D3D11_RASTERIZER_DESC1* pRasterizerDesc,
	ID3D11RasterizerState1** ppRasterizerState)
{
	return m_pDevice_->CreateRasterizerState1(pRasterizerDesc, ppRasterizerState);
}

HRESULT MyId3D11Device::CreateDeviceContextState(UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels, UINT SDKVersion, const IID& EmulatedInterface, D3D_FEATURE_LEVEL* pChosenFeatureLevel,
	ID3DDeviceContextState** ppContextState)
{
	return m_pDevice_->CreateDeviceContextState(Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState);
}

HRESULT MyId3D11Device::OpenSharedResource1(HANDLE hResource, const IID& returnedInterface, void** ppResource)
{
	return m_pDevice_->OpenSharedResource1(hResource, returnedInterface, ppResource);
}

HRESULT MyId3D11Device::OpenSharedResourceByName(LPCWSTR lpName, DWORD dwDesiredAccess, const IID& returnedInterface,
	void** ppResource)
{
	return m_pDevice_->OpenSharedResourceByName(lpName, dwDesiredAccess, returnedInterface, ppResource);
}

void MyId3D11Device::GetImmediateContext2(ID3D11DeviceContext2** ppImmediateContext)
{
	return m_pDevice_->GetImmediateContext2(ppImmediateContext);
}

HRESULT MyId3D11Device::CreateDeferredContext2(UINT ContextFlags, ID3D11DeviceContext2** ppDeferredContext)
{
	return m_pDevice_->CreateDeferredContext2(ContextFlags, ppDeferredContext);
}

void MyId3D11Device::GetResourceTiling(ID3D11Resource* pTiledResource, UINT* pNumTilesForEntireResource,
	D3D11_PACKED_MIP_DESC* pPackedMipDesc, D3D11_TILE_SHAPE* pStandardTileShapeForNonPackedMips,
	UINT* pNumSubresourceTilings, UINT FirstSubresourceTilingToGet,
	D3D11_SUBRESOURCE_TILING* pSubresourceTilingsForNonPackedMips)
{
	return m_pDevice_->GetResourceTiling(pTiledResource, pNumTilesForEntireResource,
		pPackedMipDesc, pStandardTileShapeForNonPackedMips,
		pNumSubresourceTilings, FirstSubresourceTilingToGet,
		pSubresourceTilingsForNonPackedMips);
}

HRESULT MyId3D11Device::CheckMultisampleQualityLevels1(DXGI_FORMAT Format, UINT SampleCount, UINT Flags,
	UINT* pNumQualityLevels)
{
	return m_pDevice_->CheckMultisampleQualityLevels1(Format, SampleCount, Flags,
		pNumQualityLevels);
}

HRESULT MyId3D11Device::CreateTexture2D1(const D3D11_TEXTURE2D_DESC1* pDesc1,
	const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D1** ppTexture2D)
{
	return m_pDevice_->CreateTexture2D1(pDesc1, pInitialData, ppTexture2D);
}

HRESULT MyId3D11Device::CreateTexture3D1(const D3D11_TEXTURE3D_DESC1* pDesc1,
	const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture3D1** ppTexture3D)
{
	return m_pDevice_->CreateTexture3D1(pDesc1, pInitialData, ppTexture3D);
}

HRESULT MyId3D11Device::CreateRasterizerState2(const D3D11_RASTERIZER_DESC2* pRasterizerDesc,
	ID3D11RasterizerState2** ppRasterizerState)
{
	return m_pDevice_->CreateRasterizerState2(pRasterizerDesc, ppRasterizerState);
}

HRESULT MyId3D11Device::CreateShaderResourceView1(ID3D11Resource* pResource,
	const D3D11_SHADER_RESOURCE_VIEW_DESC1* pDesc1, ID3D11ShaderResourceView1** ppSRView1)
{
	return m_pDevice_->CreateShaderResourceView1(pResource, pDesc1, ppSRView1);
}

HRESULT MyId3D11Device::CreateUnorderedAccessView1(ID3D11Resource* pResource,
	const D3D11_UNORDERED_ACCESS_VIEW_DESC1* pDesc1, ID3D11UnorderedAccessView1** ppUAView1)
{
	return m_pDevice_->CreateUnorderedAccessView1(pResource, pDesc1, ppUAView1);
}

HRESULT MyId3D11Device::CreateRenderTargetView1(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC1* pDesc1,
	ID3D11RenderTargetView1** ppRTView1)
{
	return m_pDevice_->CreateRenderTargetView1(pResource, pDesc1, ppRTView1);
}

HRESULT MyId3D11Device::CreateQuery1(const D3D11_QUERY_DESC1* pQueryDesc1, ID3D11Query1** ppQuery1)
{
	return m_pDevice_->CreateQuery1(pQueryDesc1, ppQuery1);
}

void MyId3D11Device::GetImmediateContext3(ID3D11DeviceContext3** ppImmediateContext)
{
	return m_pDevice_->GetImmediateContext3(ppImmediateContext);
}

HRESULT MyId3D11Device::CreateDeferredContext3(UINT ContextFlags, ID3D11DeviceContext3** ppDeferredContext)
{
	return m_pDevice_->CreateDeferredContext3(ContextFlags, ppDeferredContext);
}

void MyId3D11Device::WriteToSubresource(ID3D11Resource* pDstResource, UINT DstSubresource, const D3D11_BOX* pDstBox,
	const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{
	return m_pDevice_->WriteToSubresource(pDstResource, DstSubresource, pDstBox,
		 pSrcData, SrcRowPitch, SrcDepthPitch);
}

void MyId3D11Device::ReadFromSubresource(void* pDstData, UINT DstRowPitch, UINT DstDepthPitch,
	ID3D11Resource* pSrcResource, UINT SrcSubresource, const D3D11_BOX* pSrcBox)
{
	return m_pDevice_->ReadFromSubresource(pDstData, DstRowPitch, DstDepthPitch,
		pSrcResource, SrcSubresource, pSrcBox);
}

HRESULT MyId3D11Device::RegisterDeviceRemovedEvent(HANDLE hEvent, DWORD* pdwCookie)
{
	return m_pDevice_->RegisterDeviceRemovedEvent(hEvent, pdwCookie);
}

void MyId3D11Device::UnregisterDeviceRemoved(DWORD dwCookie)
{
	return m_pDevice_->UnregisterDeviceRemoved(dwCookie);
}

HRESULT MyId3D11Device::OpenSharedFence(HANDLE hFence, const IID& ReturnedInterface, void** ppFence)
{
	return m_pDevice_->OpenSharedFence(hFence, ReturnedInterface, ppFence);
}

HRESULT MyId3D11Device::CreateFence(UINT64 InitialValue, D3D11_FENCE_FLAG Flags, const IID& ReturnedInterface,
	void** ppFence)
{
	return m_pDevice_->CreateFence(InitialValue, Flags, ReturnedInterface,
		ppFence);
}

