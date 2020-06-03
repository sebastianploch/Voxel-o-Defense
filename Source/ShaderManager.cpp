#include "pch.h"
#include "ShaderManager.h"

#include "Shader.h"


ShaderManager::ShaderManager(ID3D11Device1* device) :
	m_currentShader(Shaders::NONE)
{
	InitialiseShaders(device);
}

ShaderManager::~ShaderManager()
{
	m_shaders.clear();
}

void ShaderManager::SetShader(const Shaders& shader,
							  ID3D11DeviceContext1* context)
{
	if (shader != m_currentShader)
	{
		// Update Currently Active Shader
		m_currentShader = shader;

		// Set Input Layout
		context->IASetInputLayout(m_shaders[static_cast<int>(shader)]->GetInputLayout());

		// Set Vertex Shader
		context->VSSetShader(m_shaders[static_cast<int>(shader)]->GetVertexShader(), nullptr, 0);

		// Set Pixel Shader
		context->PSSetShader(m_shaders[static_cast<int>(shader)]->GetPixelShader(), nullptr, 0);
	}
}

void ShaderManager::InitialiseShaders(ID3D11Device1* device)
{
	// Basic Shader
	m_shaders.push_back(std::make_unique<Shader>(device,
												 L"Resources/Shaders/BasicVertexShader.hlsl",
												 L"Resources/Shaders/BasicPixelShader.hlsl",
												 *DirectX::VertexPositionNormalTexture::InputElements,
												 DirectX::VertexPositionNormalTexture::InputElementCount));

	// Voxel Shader
	m_shaders.push_back(std::make_unique<Shader>(device,
												 L"Resources/Shaders/BasicVoxelVertexShader.hlsl",
												 L"Resources/Shaders/BasicVoxelPixelShader.hlsl",
												 *DirectX::VertexPositionNormalDualTexture::InputElements,
												 DirectX::VertexPositionNormalDualTexture::InputElementCount));
	// Water Shader
	m_shaders.push_back(std::make_unique<Shader>(device,
		L"Resources/Shaders/WaterVertexShader.hlsl",
		L"Resources/Shaders/WaterPixelShader.hlsl",
		*DirectX::VertexPositionNormalDualTexture::InputElements,
		DirectX::VertexPositionNormalDualTexture::InputElementCount));

	// #TODO Toon Shader 
}