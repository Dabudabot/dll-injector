#include "stdafx.h"
#include "id3d11device.h"


MyID3D11Device::MyID3D11Device(ID3D11Device** ppDevice, Overlay *overlay)
{
	m_pDevice_ = *ppDevice;
	m_overlay_ = overlay;
}

HRESULT MyID3D11Device::QueryInterface(const IID& riid, void** ppvObject)
{
	//TODO this function edit
	return m_pDevice_->QueryInterface(riid, ppvObject);
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
}

HRESULT MyID3D11Device::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
	ID3D11RenderTargetView** ppRTView)
{
}

HRESULT MyID3D11Device::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
	ID3D11DepthStencilView** ppDepthStencilView)
{
}

HRESULT MyID3D11Device::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements,
	const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
{
}

HRESULT MyID3D11Device::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
{
}

HRESULT MyID3D11Device::CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
}

HRESULT MyID3D11Device::CreateGeometryShaderWithStreamOutput(const void* pShaderBytecode, SIZE_T BytecodeLength,
	const D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, const UINT* pBufferStrides, UINT NumStrides,
	UINT RasterizedStream, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
}

HRESULT MyID3D11Device::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
}

HRESULT MyID3D11Device::CreateHullShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11HullShader** ppHullShader)
{
}

HRESULT MyID3D11Device::CreateDomainShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11DomainShader** ppDomainShader)
{
}

HRESULT MyID3D11Device::CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader)
{
}

HRESULT MyID3D11Device::CreateClassLinkage(ID3D11ClassLinkage** ppLinkage)
{
}

HRESULT MyID3D11Device::CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState)
{
}

HRESULT MyID3D11Device::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc,
	ID3D11DepthStencilState** ppDepthStencilState)
{
}

HRESULT MyID3D11Device::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc,
	ID3D11RasterizerState** ppRasterizerState)
{
}

HRESULT MyID3D11Device::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
}

HRESULT MyID3D11Device::CreateQuery(const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery)
{
}

HRESULT MyID3D11Device::CreatePredicate(const D3D11_QUERY_DESC* pPredicateDesc, ID3D11Predicate** ppPredicate)
{
}

HRESULT MyID3D11Device::CreateCounter(const D3D11_COUNTER_DESC* pCounterDesc, ID3D11Counter** ppCounter)
{
}

HRESULT MyID3D11Device::CreateDeferredContext(UINT ContextFlags, ID3D11DeviceContext** ppDeferredContext)
{
}

HRESULT MyID3D11Device::OpenSharedResource(HANDLE hResource, const IID& ReturnedInterface, void** ppResource)
{
}

HRESULT MyID3D11Device::CheckFormatSupport(DXGI_FORMAT Format, UINT* pFormatSupport)
{
}

HRESULT MyID3D11Device::CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels)
{
}

void MyID3D11Device::CheckCounterInfo(D3D11_COUNTER_INFO* pCounterInfo)
{
}

HRESULT MyID3D11Device::CheckCounter(const D3D11_COUNTER_DESC* pDesc, D3D11_COUNTER_TYPE* pType, UINT* pActiveCounters,
	LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength)
{
}

HRESULT MyID3D11Device::CheckFeatureSupport(D3D11_FEATURE Feature, void* pFeatureSupportData,
	UINT FeatureSupportDataSize)
{
}

HRESULT MyID3D11Device::GetPrivateData(const GUID& guid, UINT* pDataSize, void* pData)
{
}

HRESULT MyID3D11Device::SetPrivateData(const GUID& guid, UINT DataSize, const void* pData)
{
}

HRESULT MyID3D11Device::SetPrivateDataInterface(const GUID& guid, const IUnknown* pData)
{
}

D3D_FEATURE_LEVEL MyID3D11Device::GetFeatureLevel()
{
}

UINT MyID3D11Device::GetCreationFlags()
{
}

HRESULT MyID3D11Device::GetDeviceRemovedReason()
{
}

void MyID3D11Device::GetImmediateContext(ID3D11DeviceContext** ppImmediateContext)
{
}

HRESULT MyID3D11Device::SetExceptionMode(UINT RaiseFlags)
{
}

UINT MyID3D11Device::GetExceptionMode()
{
}

