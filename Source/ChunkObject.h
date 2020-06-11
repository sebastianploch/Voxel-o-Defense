#pragma once

#include "IGameObject.h"

class ChunkObject : public IGameObject {
public:
	ChunkObject();
	ChunkObject(const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& rotation = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));

	~ChunkObject();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext1* context,
			  ConstantBuffer& cb,
			  ID3D11Buffer* constantBuffer) override;

	static void InitTexture(const wchar_t* texturePath, ID3D11Device* device);

	VoxelMesh* GetMesh() { return &m_mesh; }
	inline DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMat; }

private:

	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> s_texture;

	VoxelMesh m_mesh = VoxelMesh();

	//Transformation
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Vector3 m_scaling;
};