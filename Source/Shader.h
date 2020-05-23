#pragma once


class Shader
{
public:
	Shader(ID3D11Device1* device,
		   const LPCTSTR& vertexShaderPath,
		   const LPCTSTR& pixelShaderPath,
		   const D3D11_INPUT_ELEMENT_DESC& elementDescription = *DirectX::VertexPositionNormalTexture::InputElements,
		   const UINT& elementCount = DirectX::VertexPositionNormalTexture::InputElementCount);

	~Shader();

	inline ID3D11InputLayout* GetInputLayout() const { return m_inputLayout.Get(); }
	inline ID3D11PixelShader* GetPixelShader() const { return m_pixelShader.Get(); }
	inline ID3D11VertexShader* GetVertexShader() const { return m_vertexShader.Get(); }

private:
	void InitialiseShader(ID3D11Device1* device,
						  const LPCTSTR& vertexShaderPath,
						  const LPCTSTR& pixelShaderPath,
						  const D3D11_INPUT_ELEMENT_DESC& elementDescription,
						  const UINT& elementCount);

	void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC& elementDescription,
						   const UINT& elementCount,
						   ID3DBlob* vsBlob,
						   ID3D11Device1* device);

	ID3DBlob* CreateVertexShader(const LPCTSTR& vertexShaderPath,
								 ID3D11Device1* device);

	ID3DBlob* CreatePixelShader(const LPCTSTR& pixelShaderPath,
							    ID3D11Device1* device);

	static void ReleaseBlobs(ID3DBlob& vsBlob,
							 ID3DBlob& psBlob);

	static HRESULT CompileShader(const LPCTSTR& path,
								 const LPCSTR& entryPoint,
								 const LPCSTR& profile,
								 ID3DBlob** blob);

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
};