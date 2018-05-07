#pragma once
#include "overlay.h"

class MyID3D11Device : public ID3D11Device
{
	ID3D11Device* m_pDevice_ = nullptr;

public:
	Overlay * m_overlay = nullptr;

	MyID3D11Device(ID3D11Device** ppDevice, Overlay *overlay);
	~MyID3D11Device() = default;

	HRESULT QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG AddRef() override;
	ULONG Release() override;
	HRESULT CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
		ID3D11Buffer** ppBuffer) override;
	HRESULT CreateTexture1D(const D3D11_TEXTURE1D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
		ID3D11Texture1D** ppTexture1D) override;
	HRESULT CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
		ID3D11Texture2D** ppTexture2D) override;
	HRESULT CreateTexture3D(const D3D11_TEXTURE3D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData,
		ID3D11Texture3D** ppTexture3D) override;
	HRESULT CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc,
		ID3D11ShaderResourceView** ppSRView) override;
	HRESULT CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc,
		ID3D11UnorderedAccessView** ppUAView) override;
	HRESULT CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
		ID3D11RenderTargetView** ppRTView) override;
	HRESULT CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
		ID3D11DepthStencilView** ppDepthStencilView) override;
	HRESULT CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements,
		const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout) override;
	HRESULT CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage,
		ID3D11VertexShader** ppVertexShader) override;
	HRESULT CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage,
		ID3D11GeometryShader** ppGeometryShader) override;
	HRESULT CreateGeometryShaderWithStreamOutput(const void* pShaderBytecode, SIZE_T BytecodeLength,
		const D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, const UINT* pBufferStrides, UINT NumStrides,
		UINT RasterizedStream, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader) override;
	HRESULT CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage,
		ID3D11PixelShader** ppPixelShader) override;
	HRESULT CreateHullShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage,
		ID3D11HullShader** ppHullShader) override;
	HRESULT CreateDomainShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage,
		ID3D11DomainShader** ppDomainShader) override;
	HRESULT CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage,
		ID3D11ComputeShader** ppComputeShader) override;
	HRESULT CreateClassLinkage(ID3D11ClassLinkage** ppLinkage) override;
	HRESULT CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState) override;
	HRESULT CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc,
		ID3D11DepthStencilState** ppDepthStencilState) override;
	HRESULT CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
	override;
	HRESULT CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState) override;
	HRESULT CreateQuery(const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery) override;
	HRESULT CreatePredicate(const D3D11_QUERY_DESC* pPredicateDesc, ID3D11Predicate** ppPredicate) override;
	HRESULT CreateCounter(const D3D11_COUNTER_DESC* pCounterDesc, ID3D11Counter** ppCounter) override;
	HRESULT CreateDeferredContext(UINT ContextFlags, ID3D11DeviceContext** ppDeferredContext) override;
	HRESULT OpenSharedResource(HANDLE hResource, const IID& ReturnedInterface, void** ppResource) override;
	HRESULT CheckFormatSupport(DXGI_FORMAT Format, UINT* pFormatSupport) override;
	HRESULT CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels) override;
	void CheckCounterInfo(D3D11_COUNTER_INFO* pCounterInfo) override;
	HRESULT CheckCounter(const D3D11_COUNTER_DESC* pDesc, D3D11_COUNTER_TYPE* pType, UINT* pActiveCounters, LPSTR szName,
		UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength) override;
	HRESULT CheckFeatureSupport(D3D11_FEATURE Feature, void* pFeatureSupportData, UINT FeatureSupportDataSize) override;
	HRESULT GetPrivateData(const GUID& guid, UINT* pDataSize, void* pData) override;
	HRESULT SetPrivateData(const GUID& guid, UINT DataSize, const void* pData) override;
	HRESULT SetPrivateDataInterface(const GUID& guid, const IUnknown* pData) override;
	D3D_FEATURE_LEVEL GetFeatureLevel() override;
	UINT GetCreationFlags() override;
	HRESULT GetDeviceRemovedReason() override;
	void GetImmediateContext(ID3D11DeviceContext** ppImmediateContext) override;
	HRESULT SetExceptionMode(UINT RaiseFlags) override;
	UINT GetExceptionMode() override;
};

