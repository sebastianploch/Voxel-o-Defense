#pragma once

#include "IGameObject.h"

struct ParticleGeometry
{
	Microsoft::WRL::ComPtr<ID3D11Buffer>             particleVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>             particleIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleDebugTexture;

	DirectX::VertexPositionNormalTexture				particleVertices[24];
	WORD												particleIndices[36];
};

class Particle : public IGameObject
{
public:
	Particle();

	Particle(const ParticleGeometry& geometry,
					const DirectX::SimpleMath::Vector3& position,
					const DirectX::SimpleMath::Vector3& rotation = DirectX::SimpleMath::Vector3::Zero,
					const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
					float lifetime=10.0f);

	~Particle();

	void setForce(const DirectX::SimpleMath::Vector3& force);
	DirectX::SimpleMath::Vector3 getForce();

	bool isAlive();

	float getMaxLifetime();
	float getLifetime();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext* context) override;

	// Static Data Init

	static void InitBuffers(ID3D11Device* device);
	static void InitDebugTexture(const wchar_t* texturePath, ID3D11Device* device);

private:
	
	ParticleGeometry m_geometry;

	// Transformation

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Vector3 m_scaling;

	// Particle Properties
	float m_maxLifeTime;
	float m_lifetime;
	DirectX::SimpleMath::Vector3 m_force;
};