#include "pch.h"
#include "Shader.h"

static constexpr LPCSTR s_vertexEntry = "VSMain";
static constexpr LPCSTR s_vertexVersion = "vs_4_0_level_9_3";
static constexpr LPCSTR s_pixelEntry = "PSMain";
static constexpr LPCSTR s_pixelVersion = "ps_4_0_level_9_3";


Shader::Shader(ID3D11Device1* device,
			   const LPCTSTR& vertexShaderPath,
			   const LPCTSTR& pixelShaderPath,
			   const D3D11_INPUT_ELEMENT_DESC& elementDescription,
			   const UINT& elementCount)
{
	InitialiseShader(device,
					 vertexShaderPath,
					 pixelShaderPath,
					 elementDescription,
					 elementCount);
}

Shader::~Shader()
{
	m_inputLayout.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
}

void Shader::InitialiseShader(ID3D11Device1* device,
							  const LPCTSTR& vertexShaderPath,
							  const LPCTSTR& pixelShaderPath,
							  const D3D11_INPUT_ELEMENT_DESC& elementDescription,
							  const UINT& elementCount)
{
	ID3DBlob* vsBlob = CreateVertexShader(vertexShaderPath, device);
	ID3DBlob* psBlob = CreatePixelShader(pixelShaderPath, device);

	CreateInputLayout(elementDescription,
					  elementCount,
					  vsBlob, device);

	ReleaseBlobs(*vsBlob,
				 *psBlob);
}

void Shader::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC& elementDescription,
							   const UINT& elementCount,
							   ID3DBlob* vsBlob,
							   ID3D11Device1* device)
{
	device->CreateInputLayout(&elementDescription,
							  elementCount,
							  vsBlob->GetBufferPointer(),
							  vsBlob->GetBufferSize(),
							  m_inputLayout.GetAddressOf());
}

ID3DBlob* Shader::CreateVertexShader(const LPCTSTR& vertexShaderPath,
									 ID3D11Device1* device)
{
	ID3DBlob* vsBlob = nullptr;

	// Compile vertex shader byte code
	DX::ThrowIfFailed(CompileShader(vertexShaderPath,
									s_vertexEntry,
									s_vertexVersion,
									&vsBlob));

	// Attach compiled vertex Shader
	DX::ThrowIfFailed(device->CreateVertexShader(vsBlob->GetBufferPointer(),
												 vsBlob->GetBufferSize(),
												 nullptr,
												 m_vertexShader.ReleaseAndGetAddressOf()));

	return vsBlob;
}

ID3DBlob* Shader::CreatePixelShader(const LPCTSTR& pixelShaderPath,
									ID3D11Device1* device)
{
	ID3DBlob* psBlob = nullptr;

	// Compile pixel shader byte code
	DX::ThrowIfFailed(CompileShader(pixelShaderPath,
									s_pixelEntry,
									s_pixelVersion,
									&psBlob));

	// Attach compiled pixel Shader
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob->GetBufferPointer(),
											    psBlob->GetBufferSize(),
												nullptr,
												m_pixelShader.ReleaseAndGetAddressOf()));

	return psBlob;
}

void Shader::ReleaseBlobs(ID3DBlob& vsBlob,
						  ID3DBlob& psBlob)
{
	vsBlob.Release();
	psBlob.Release();
}

HRESULT Shader::CompileShader(const LPCTSTR& path,
							  const LPCSTR& entryPoint,
							  const LPCSTR& profile,
							  ID3DBlob** blob)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif // _DEBUG

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(path,
									NULL,
									D3D_COMPILE_STANDARD_FILE_INCLUDE,
									entryPoint,
									profile,
									flags,
									0,
									&shaderBlob,
									&errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}

		if (shaderBlob)
		{
			shaderBlob->Release();
		}

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}