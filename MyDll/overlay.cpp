// overlay.cpp : implementation of overlay drawing
#include "stdafx.h"
#include "overlay.h"

// Vertex struct
struct Vertex
{
	XMFLOAT3 m_pos;
};

Overlay::Overlay(ID3D11Device** pDevice, ID3D11DeviceContext** pContext)
{
	m_pD3DDevice = *pDevice;
	m_pD3DContext = *pContext;
	m_pVsRedColor = nullptr;
	m_pPsRedColor = nullptr;
	m_pInputLayout = nullptr;
	m_pVertexBuffer = nullptr;
}

bool Overlay::compileShader(const LPCWSTR szFilePath, const LPCSTR szFunc, const LPCSTR szShaderModel, ID3DBlob** buffer)
{
	// Set flags
	const DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* errBuffer = nullptr;
	const auto hr = ::D3DX11CompileFromFile(
		szFilePath, nullptr, nullptr, szFunc, szShaderModel,
		flags, 0, nullptr, buffer, &errBuffer, nullptr);

	// Check for errors
	if (FAILED(hr)) {
		if (errBuffer != nullptr) {
			::OutputDebugStringA(static_cast<char*>(errBuffer->GetBufferPointer()));
			errBuffer->Release();
		}
		return false;
	}

	// Cleanup
	if (errBuffer != nullptr)
		errBuffer->Release();
	return true;
}

bool Overlay::loadContent()
{
	const auto shaderPath = L"C:\\Users\\Daulet\\source\\repos\\RE-S18\\RE-S18\\MyDll\\ShaderRedColor.fx";
	// Compile vertex shader
	ID3DBlob* pVsBuffer = nullptr;
	auto res = compileShader(shaderPath, "VS_Main", "vs_4_0", &pVsBuffer);
	if (!res) {
		MessageBox(nullptr, L"Unable to load vertex shader", L"ERROR MyDll.dll", MB_OK);
		return false;
	}

	auto hr = m_pD3DDevice->CreateVertexShader(
		pVsBuffer->GetBufferPointer(),
		pVsBuffer->GetBufferSize(),
		nullptr, &m_pVsRedColor);
	if (FAILED(hr)) {
		if (pVsBuffer)
			pVsBuffer->Release();
		return false;
	}

	// Define input layout
	D3D11_INPUT_ELEMENT_DESC shaderInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	const UINT numLayoutElements = ARRAYSIZE(shaderInputLayout);

	// Create input layout
	hr = m_pD3DDevice->CreateInputLayout(
		shaderInputLayout, numLayoutElements,
		pVsBuffer->GetBufferPointer(),
		pVsBuffer->GetBufferSize(),
		&m_pInputLayout);
	if (FAILED(hr)) {
		return false;
	}

	// Release VS buffer
	pVsBuffer->Release();
	pVsBuffer = nullptr;

	// Compile pixel shader
	ID3DBlob* pPsBuffer = nullptr;
	res = compileShader(shaderPath, "PS_Main", "ps_4_0", &pPsBuffer);
	if (res == false) {
		MessageBox(nullptr, L"Unable to load pixel shader", L"ERROR MyDll.dll", MB_OK);
		return false;
	}

	// Create pixel shader
	hr = m_pD3DDevice->CreatePixelShader(
		pPsBuffer->GetBufferPointer(),
		pPsBuffer->GetBufferSize(),
		0, &m_pPsRedColor);
	if (FAILED(hr)) {
		return false;
	}

	// Cleanup PS buffer
	pPsBuffer->Release();
	pPsBuffer = nullptr;

	// Define triangle
	Vertex vertices[] =
	{
		{ XMFLOAT3(0.1f,  0.1f, 1.0f) },
		{ XMFLOAT3(0.1f, -0.1f, 1.0f) },
		{ XMFLOAT3(-0.1f, -0.1f, 1.0f) },

		{ XMFLOAT3(-0.1f, -0.1f, 1.0f) },
		{ XMFLOAT3(-0.1f,  0.1f, 1.0f) },
		{ XMFLOAT3(0.1f,  0.1f, 1.0f) },
	};

	// Vertex description
	D3D11_BUFFER_DESC vertexDesc;
	::ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(Vertex) * 6;

	// Resource data
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	// Create vertex buffer
	hr = m_pD3DDevice->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);

	if (FAILED(hr)) {  // NOLINT(readability-simplify-boolean-expr)
		return false;  // NOLINT(readability-simplify-boolean-expr)
	}

	MessageBox(nullptr, L"Overlay draw", L"MyDll.dll", MB_OK);

	return true;
}

void Overlay::unloadContent()
{
	// Cleanup
	if (m_pVsRedColor)
		m_pVsRedColor->Release();
	m_pVsRedColor = nullptr;
	if (m_pPsRedColor)
		m_pPsRedColor->Release();
	m_pPsRedColor = nullptr;
	if (m_pInputLayout)
		m_pInputLayout->Release();
	m_pInputLayout = nullptr;
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();
	m_pVertexBuffer = nullptr;
}

void Overlay::render() const
{
	// Check if D3D is ready
	if (m_pD3DContext == nullptr)
		return;

	// Stride and offset
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Set vertex buffer
	m_pD3DContext->IASetInputLayout(m_pInputLayout);
	m_pD3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set shaders
	m_pD3DContext->VSSetShader(m_pVsRedColor, nullptr, 0);
	m_pD3DContext->PSSetShader(m_pPsRedColor, nullptr, 0);

	// Draw triangle
	m_pD3DContext->Draw(6, 0);
}
