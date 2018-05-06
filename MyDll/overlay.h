#pragma once
/**
 * \brief Overlay class draws overlay on image
 */
class Overlay  // NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
{
	// Ctors
public:
	/**
	 * \brief Ctor that initializes device and context, they required to draw
	 * \param pDevice dx device
	 * \param pContext dx context
	 */
	Overlay(ID3D11Device** pDevice, ID3D11DeviceContext** pContext);
	/**
	 * \brief deleted default ctor
	 */
	Overlay() = delete;
	/**
	 * \brief default copy ctor
	 */
	Overlay(_In_ const Overlay &) = default;
	/**
	 * \brief default move ctor
	 */
	Overlay(_In_ Overlay &&) = default;
	/**
	 * \brief default dtor
	 */
	~Overlay() = default;

	// Functions
public:
	/**
	 * \brief compiles loaded shader
	 * \param szFilePath path to shader file
	 * \param szFunc shader function existing in shader function
	 * \param szShaderModel version of the shader
	 * \param buffer shader/pixel buffer
	 * \return true in case of success
	 */
	static bool compileShader(_In_ LPCWSTR szFilePath, 
		_In_ LPCSTR szFunc, 
		_In_ LPCSTR szShaderModel, 
		_Out_ ID3DBlob** buffer);
	/**
	 * \brief loads shader file, 
	 * compiles/creates vertex shader, 
	 * creates input layout, 
	 * compiles/create pixel shader,
	 * defines vertexies and create buffer for it 
	 * \return true in case of success
	 */
	bool loadContent();
	/**
	 * \brief unloads loaded shaders and buffers
	 */
	void unloadContent();
	/**
	 * \brief draw overlay
	 */
	void render() const;

	// Attributes
public:
	ID3D11Device*			m_pD3DDevice{};
	ID3D11DeviceContext*	m_pD3DContext{};

	ID3D11VertexShader*		m_pVsRedColor{};
	ID3D11PixelShader*		m_pPsRedColor{};
	ID3D11InputLayout*		m_pInputLayout{};
	ID3D11Buffer*			m_pVertexBuffer{};

	// Operators
public:
	Overlay & Overlay :: operator= (const Overlay &) = default;
	Overlay & Overlay :: operator= (Overlay &&) = default;
};

