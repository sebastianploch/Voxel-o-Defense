#pragma once

class IGameObject
{
public:
	virtual ~IGameObject() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(ID3D11DeviceContext1* context,
					  ConstantBuffer& cb,
					  ID3D11Buffer* constantBuffer) = 0;

	inline Shaders GetShaderType() const { return m_shaderType; }

	inline DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMat; }
	inline void SetWorldMatrix(const DirectX::SimpleMath::Matrix& worldMat) { m_worldMat = worldMat; }

protected:
	IGameObject() = default;

protected:
	// Shader to be applied on object
	Shaders						m_shaderType{ Shaders::BASIC_SHADER };

	// World Transformation Matrix
	DirectX::SimpleMath::Matrix m_worldMat{ DirectX::SimpleMath::Matrix::Identity };
};