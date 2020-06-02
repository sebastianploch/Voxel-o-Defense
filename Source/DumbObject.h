#pragma once
#include "IGameObject.h"
class DumbObject : public IGameObject
{
public:
	DumbObject(DirectX::SimpleMath::Vector3& position ,
		DirectX::SimpleMath::Vector3& rotation,
		DirectX::SimpleMath::Vector3& scale,
		std::string filePath, 
		std::string name
		);

	~DumbObject();

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* context);

	// Static Data Init
	static void InitBuffers(ID3D11Device* device);
	static void InitDebugTexture(const wchar_t* texturePath, ID3D11Device* device);
	void SetPosition(DirectX::SimpleMath::Vector3 pos);

	inline DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMatrix; }

private:
	// Shared static data

	static Microsoft::WRL::ComPtr<ID3D11Buffer>             s_cubeVertexBuffer;
	static Microsoft::WRL::ComPtr<ID3D11Buffer>             s_cubeIndexBuffer;
	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> s_debugTexture;

	static DirectX::VertexPositionNormalTexture				s_cubeVertices[24];
	static WORD												s_cubeIndices[36];

	GameObjectCfg m_config;

	//Transformation
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Vector3 m_scaling;
	DirectX::SimpleMath::Matrix m_worldMatrix;

};

