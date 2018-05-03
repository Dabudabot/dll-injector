#pragma once
class Overlay
{
public:
	Overlay() = default;
	~Overlay() = default;

	static bool compileShader(_In_ LPCWSTR szFilePath, _In_ LPCSTR szFunc, _In_ LPCSTR szShaderModel, _Out_ ID3DBlob** buffer);
	bool loadContent();
	void unloadContent();
	void render() const;

	ID3D11Device*			m_pD3DDevice;
	ID3D11DeviceContext*	m_pD3DContext;

	ID3D11VertexShader*		m_pVsRedColor;
	ID3D11PixelShader*		m_pPsRedColor;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11Buffer*			m_pVertexBuffer;
};

