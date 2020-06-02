#pragma once

#include "Particle.h"

class ParticleEmitter
{
public:

	ParticleEmitter(ID3D11Device* device, const wchar_t* particleTexturePath);
	~ParticleEmitter();

	void setPosition(const DirectX::SimpleMath::Vector3& position);
	DirectX::SimpleMath::Vector3 getPosition();

	void setParticleGeometry(const ParticleGeometry& geometry);

	void setParticleRotation(const DirectX::SimpleMath::Vector3& rotation);
	DirectX::SimpleMath::Vector3 getParticleRotation();

	void setParticleRotationVariation(const DirectX::SimpleMath::Vector3& rotationVariation);
	DirectX::SimpleMath::Vector3 getParticleRotationVariation();

	void setParticleScale(const DirectX::SimpleMath::Vector3& scale);
	DirectX::SimpleMath::Vector3 getParticleScale();

	void setParticleScaleVariation(const DirectX::SimpleMath::Vector3& scaleVariation);
	DirectX::SimpleMath::Vector3 getParticleScaleVariation();

	void setParticleLifetime(float lifetime);
	float getParticleLifetime();

	void setParticleLifetimeVariation(float lifetimeVariation);
	float getParticleLifetimeVariation();

	void setMaxParticles(unsigned int maxParticles);
	unsigned int getMaxParticles();

	void setForce(const DirectX::SimpleMath::Vector3& force);
	DirectX::SimpleMath::Vector3 getForce();

	void setForceRange(const DirectX::SimpleMath::Vector3& forceRange);
	DirectX::SimpleMath::Vector3 getForceRange();

	std::vector<Particle*> getParticles();

	void setSpawnDelay(float spawnDelay);
	float getSpawnDelay();

	void updateParticles(float deltaTime);
	void drawParticles(ID3D11DeviceContext* context);

private:

	DirectX::SimpleMath::Vector3 m_position;

	ParticleGeometry m_particleGeometry;

	DirectX::SimpleMath::Vector3 m_particleRotation;
	DirectX::SimpleMath::Vector3 m_particleRotationVariation;
	DirectX::SimpleMath::Vector3 m_particleScale;
	DirectX::SimpleMath::Vector3 m_particleScaleVariation;
	float m_particleLifetime;
	float m_particleLifetimeVariation;

	unsigned int m_maxParticles;
	DirectX::SimpleMath::Vector3 m_force;
	DirectX::SimpleMath::Vector3 m_forceRange;
	std::vector<Particle*> m_particles;

	float randFloat(float low, float high);

	float m_spawnDelay;
	float m_spawnTimer = 0.0f;
};