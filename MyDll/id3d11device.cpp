#include "stdafx.h"
#include "id3d11device.h"


MyID3D11Device::MyID3D11Device(ID3D11Device** ppDevice, Overlay *overlay)
{
	m_pDevice_ = *ppDevice;
	m_overlay = overlay;
}

HRESULT MyID3D11Device::QueryInterface(const IID& riid, void** ppvObject)
{
	const auto result = m_pDevice_->QueryInterface(riid, ppvObject);

	if (riid == __uuidof(ID3D11Device1))
	{
		m_pDevice1_ = new MyID3D11Device1(ppvObject, m_overlay);
		ppvObject = m_pDevice1_;
	} 
	else if (riid == __uuidof(ID3D11Device2))
	{
		m_pDevice2_ = new MyID3D11Device2(ppvObject, m_overlay);
		ppvObject = m_pDevice2_;
	}
	else if (riid == __uuidof(ID3D11Device3))
	{
		m_pDevice3_ = new MyID3D11Device3(ppvObject, m_overlay);
		ppvObject = m_pDevice3_;
	}
	else if (riid == __uuidof(ID3D11Device4))
	{
		m_pDevice4_ = new MyID3D11Device4(ppvObject, m_overlay);
		ppvObject = m_pDevice4_;
	}
	else if (riid == __uuidof(ID3D11Device5))
	{
		m_pDevice5_ = new MyID3D11Device5(ppvObject, m_overlay);
		ppvObject = m_pDevice5_;
	}
	else if (riid == __uuidof(IDXGIDevice))
	{
		m_pIdxgiDevice_ = new MyIDXGIDevice(ppvObject, m_overlay);
		ppvObject = m_pIdxgiDevice_;
	}
	else if (riid == __uuidof(IDXGIDevice1))
	{
		m_pIdxgiDevice1_ = new MyIDXGIDevice1(ppvObject, m_overlay);
		ppvObject = m_pIdxgiDevice1_;
	}
	else if (riid == __uuidof(IDXGIDevice2))
	{
		m_pIdxgiDevice2_ = new MyIDXGIDevice2(ppvObject, m_overlay);
		ppvObject = m_pIdxgiDevice2_;
	}

	return result;
}

ULONG MyID3D11Device::AddRef()
{
	return m_pDevice_->AddRef();
}

ULONG MyID3D11Device::Release()
{
	//TODO i need this
	return m_pDevice_->Release();
}

HRESULT MyID3D11Device::CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
	ID3D11Buffer** ppBuffer)
{
	return m_pDevice_->CreateBuffer(pDesc, pInitialData, ppBuffer);
}

HRESULT MyID3D11Device::CreateTexture1D(const D3D11_TEXTURE1D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
	ID3D11Texture1D** ppTexture1D)
{
	return m_pDevice_->CreateTexture1D(pDesc, pInitialData, ppTexture1D);
}

HRESULT MyID3D11Device::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
	ID3D11Texture2D** ppTexture2D)
{
	return m_pDevice_->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
}

HRESULT MyID3D11Device::CreateTexture3D(const D3D11_TEXTURE3D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
	ID3D11Texture3D** ppTexture3D)
{
	return m_pDevice_->CreateTexture3D(pDesc, pInitialData, ppTexture3D);
}

HRESULT MyID3D11Device::CreateShaderResourceView(ID3D11Resource* pResource,
	const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
{
	return m_pDevice_->CreateShaderResourceView(pResource, pDesc, ppSRView);
}

HRESULT MyID3D11Device::CreateUnorderedAccessView(ID3D11Resource* pResource,
	const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
{
	return m_pDevice_->CreateUnorderedAccessView(pResource, pDesc, ppUAView);
}

HRESULT MyID3D11Device::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
	ID3D11RenderTargetView** ppRTView)
{
	return m_pDevice_->CreateRenderTargetView(pResource, pDesc, ppRTView);
}

HRESULT MyID3D11Device::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
	ID3D11DepthStencilView** ppDepthStencilView)
{
	return m_pDevice_->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
}

HRESULT MyID3D11Device::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements,
	const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
{
	return m_pDevice_->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

HRESULT MyID3D11Device::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
{
	return m_pDevice_->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

HRESULT MyID3D11Device::CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
	return m_pDevice_->CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
}

HRESULT MyID3D11Device::CreateGeometryShaderWithStreamOutput(const void* pShaderBytecode, SIZE_T BytecodeLength,
	const D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, const UINT* pBufferStrides, UINT NumStrides,
	UINT RasterizedStream, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
	return m_pDevice_->CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength,
		pSODeclaration,  NumEntries,  pBufferStrides,  NumStrides,
		RasterizedStream, pClassLinkage, ppGeometryShader);
}

HRESULT MyID3D11Device::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
	return m_pDevice_->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

HRESULT MyID3D11Device::CreateHullShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11HullShader** ppHullShader)
{
	return m_pDevice_->CreateHullShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
}

HRESULT MyID3D11Device::CreateDomainShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11DomainShader** ppDomainShader)
{
	return m_pDevice_->CreateDomainShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
}

HRESULT MyID3D11Device::CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader)
{
	return m_pDevice_->CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
}

HRESULT MyID3D11Device::CreateClassLinkage(ID3D11ClassLinkage** ppLinkage)
{
	return m_pDevice_->CreateClassLinkage(ppLinkage);
}

HRESULT MyID3D11Device::CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState)
{
	return m_pDevice_->CreateBlendState(pBlendStateDesc, ppBlendState);
}

HRESULT MyID3D11Device::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc,
	ID3D11DepthStencilState** ppDepthStencilState)
{
	return m_pDevice_->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
}

HRESULT MyID3D11Device::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc,
	ID3D11RasterizerState** ppRasterizerState)
{
	return m_pDevice_->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
}

HRESULT MyID3D11Device::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
	return m_pDevice_->CreateSamplerState(pSamplerDesc, ppSamplerState);
}

HRESULT MyID3D11Device::CreateQuery(const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery)
{
	return m_pDevice_->CreateQuery(pQueryDesc, ppQuery);
}

HRESULT MyID3D11Device::CreatePredicate(const D3D11_QUERY_DESC* pPredicateDesc, ID3D11Predicate** ppPredicate)
{
	return m_pDevice_->CreatePredicate(pPredicateDesc, ppPredicate);
}

HRESULT MyID3D11Device::CreateCounter(const D3D11_COUNTER_DESC* pCounterDesc, ID3D11Counter** ppCounter)
{
	return m_pDevice_->CreateCounter(pCounterDesc, ppCounter);
}

HRESULT MyID3D11Device::CreateDeferredContext(UINT ContextFlags, ID3D11DeviceContext** ppDeferredContext)
{
	return m_pDevice_->CreateDeferredContext(ContextFlags, ppDeferredContext);
}

HRESULT MyID3D11Device::OpenSharedResource(HANDLE hResource, const IID& ReturnedInterface, void** ppResource)
{
	return m_pDevice_->OpenSharedResource(hResource, ReturnedInterface, ppResource);
}

HRESULT MyID3D11Device::CheckFormatSupport(DXGI_FORMAT Format, UINT* pFormatSupport)
{
	return m_pDevice_->CheckFormatSupport(Format, pFormatSupport);
}

HRESULT MyID3D11Device::CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels)
{
	return m_pDevice_->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
}

void MyID3D11Device::CheckCounterInfo(D3D11_COUNTER_INFO* pCounterInfo)
{
	m_pDevice_->CheckCounterInfo(pCounterInfo);
}

HRESULT MyID3D11Device::CheckCounter(const D3D11_COUNTER_DESC* pDesc, D3D11_COUNTER_TYPE* pType, UINT* pActiveCounters,
	LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength)
{
	return m_pDevice_->CheckCounter(pDesc, pType,  pActiveCounters,
		szName,  pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
}

HRESULT MyID3D11Device::CheckFeatureSupport(D3D11_FEATURE Feature, void* pFeatureSupportData,
	UINT FeatureSupportDataSize)
{
	return m_pDevice_->CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize);
}

HRESULT MyID3D11Device::GetPrivateData(const GUID& guid, UINT* pDataSize, void* pData)
{
	return m_pDevice_->GetPrivateData(guid, pDataSize, pData);
}

HRESULT MyID3D11Device::SetPrivateData(const GUID& guid, UINT DataSize, const void* pData)
{
	return m_pDevice_->SetPrivateData(guid, DataSize, pData);
}

HRESULT MyID3D11Device::SetPrivateDataInterface(const GUID& guid, const IUnknown* pData)
{
	return m_pDevice_->SetPrivateDataInterface(guid, pData);
}

D3D_FEATURE_LEVEL MyID3D11Device::GetFeatureLevel()
{
	return m_pDevice_->GetFeatureLevel();
}

UINT MyID3D11Device::GetCreationFlags()
{
	return m_pDevice_->GetCreationFlags();
}

HRESULT MyID3D11Device::GetDeviceRemovedReason()
{
	return m_pDevice_->GetDeviceRemovedReason();
}

void MyID3D11Device::GetImmediateContext(ID3D11DeviceContext** ppImmediateContext)
{
	m_pDevice_->GetImmediateContext(ppImmediateContext);
}

HRESULT MyID3D11Device::SetExceptionMode(UINT RaiseFlags)
{
	return m_pDevice_->SetExceptionMode(RaiseFlags);
}

UINT MyID3D11Device::GetExceptionMode()
{
	return m_pDevice_->GetExceptionMode();
}

