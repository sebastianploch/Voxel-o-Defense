#pragma once

#include "IGameObject.h"


class DebugSimpleCube : public IGameObject
{
public:
	DebugSimpleCube();

	DebugSimpleCube(const DirectX::SimpleMath::Vector3& position,
					const DirectX::SimpleMath::Vector3& rotation = DirectX::SimpleMath::Vector3::Zero,
					const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));

	~DebugSimpleCube();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext* context) override;

	// Static Data Init

	static void InitBuffers(ID3D11Device* device);
	static void InitDebugTexture(const wchar_t* texturePath, ID3D11Device* device);

private:
	// Shared static data

	static Microsoft::WRL::ComPtr<ID3D11Buffer>             s_cubeVertexBuffer;
	static Microsoft::WRL::ComPtr<ID3D11Buffer>             s_cubeIndexBuffer;
	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> s_debugTexture;

	static DirectX::VertexPositionNormalTexture				s_cubeVertices[24];
	static WORD												s_cubeIndices[36];

	// Transformation

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Vector3 m_scaling;
};