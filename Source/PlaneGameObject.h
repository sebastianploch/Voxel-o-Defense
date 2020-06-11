#pragma once

#include "IGameObject.h"


class PlaneGameObject : public IGameObject
{
public:
	PlaneGameObject();

	PlaneGameObject(const DirectX::SimpleMath::Vector3& position,
					const DirectX::SimpleMath::Vector3& rotation = DirectX::SimpleMath::Vector3::Zero,
					const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));

	~PlaneGameObject();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext1* context,
			  ConstantBuffer& cb,
			  ID3D11Buffer* constantBuffer) override;

	// Static Data Init
	static void InitMeshDataAndBuffers(DirectX::SimpleMath::Vector2Int planeSize, ID3D11Device* device);
	static void InitDebugTexture(const wchar_t* texturePath, ID3D11Device* device);

private:
	static std::vector<DirectX::VertexPositionNormalTexture> GeneratePlaneVertices(int width, int depth);
	static std::vector<unsigned short> GeneratePlaneIndices(int width, int depth);

	static int s_indexCount;
	static DirectX::VertexPositionNormalTexture* s_planeVertices;
	static unsigned short* s_planeIndices;

	// Shared static data
	static Microsoft::WRL::ComPtr<ID3D11Buffer>             s_planeVertexBuffer;
	static Microsoft::WRL::ComPtr<ID3D11Buffer>             s_planeIndexBuffer;
	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> s_texture;

	GameObjectCfg m_config;

	//Transformation
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Vector3 m_scaling;
};