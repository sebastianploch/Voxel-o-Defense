#include "pch.h"
#include "ParticleEmitter.h"

using DirectX::SimpleMath::Vector3;

ParticleEmitter::ParticleEmitter(ID3D11Device* device, const wchar_t* particleTexturePath) :
	m_position(Vector3()),
	m_particleRotation(Vector3()),
	m_particleRotationVariation(Vector3()),
	m_particleScale(Vector3(1.0f, 1.0f, 1.0f)),
	m_particleScaleVariation(Vector3()),
	m_particleLifetime(10.0f),
	m_particleLifetimeVariation(0.0f)
{
	m_maxParticles = 100;
	m_force = Vector3(0.0f, 0.2f, 0.0f);
	m_forceRange = Vector3();
	m_spawnDelay = 1.0f;

	m_device = device;
	m_particleTexturePath = particleTexturePath;
}

ParticleEmitter::~ParticleEmitter()
{
	for (Particle* p : m_particles)
	{
		delete p;
	}
}

void ParticleEmitter::setPosition(const DirectX::SimpleMath::Vector3& position)
{
	m_position = position;
}

DirectX::SimpleMath::Vector3 ParticleEmitter::getPosition()
{
	return m_position;
}

void ParticleEmitter::setParticleGeometry(const ParticleGeometry& geometry)
{
	m_particleGeometry = geometry;
}

void ParticleEmitter::setParticleRotation(const DirectX::SimpleMath::Vector3& rotation)
{
	m_particleRotation = rotation;
}

DirectX::SimpleMath::Vector3 ParticleEmitter::getParticleRotation()
{
	return m_particleRotation;
}

void ParticleEmitter::setParticleRotationVariation(const DirectX::SimpleMath::Vector3& rotationVariation)
{
	m_particleRotationVariation = rotationVariation;
}

DirectX::SimpleMath::Vector3 ParticleEmitter::getParticleRotationVariation()
{
	return m_particleRotationVariation;
}

void ParticleEmitter::setParticleScale(const DirectX::SimpleMath::Vector3& scale)
{
	m_particleScale = scale;
}

DirectX::SimpleMath::Vector3 ParticleEmitter::getParticleScale()
{
	return m_particleScale;
}

void ParticleEmitter::setParticleScaleVariation(const DirectX::SimpleMath::Vector3& scaleVariation)
{
	m_particleScaleVariation = scaleVariation;
}

DirectX::SimpleMath::Vector3 ParticleEmitter::getParticleScaleVariation()
{
	return m_particleScaleVariation;
}

void ParticleEmitter::setParticleLifetime(float lifetime)
{
	m_particleLifetime = lifetime;
}

float ParticleEmitter::getParticleLifetime()
{
	return m_particleLifetime;
}

void ParticleEmitter::setParticleLifetimeVariation(float lifetimeVariation)
{
	m_particleLifetimeVariation = lifetimeVariation;
}

float ParticleEmitter::getParticleLifetimeVariation()
{
	return m_particleLifetimeVariation;
}

void ParticleEmitter::setMaxParticles(unsigned int maxParticles)
{
	m_maxParticles = maxParticles;
}

unsigned int ParticleEmitter::getMaxParticles()
{
	return m_maxParticles;
}

void ParticleEmitter::setForce(const DirectX::SimpleMath::Vector3& force)
{
	m_force = force;
}

DirectX::SimpleMath::Vector3 ParticleEmitter::getForce()
{
	return m_force;
}

void ParticleEmitter::setForceRange(const DirectX::SimpleMath::Vector3& forceRange)
{
	m_forceRange = forceRange;
}

DirectX::SimpleMath::Vector3 ParticleEmitter::getForceRange()
{
	return m_forceRange;
}

std::vector<Particle*> ParticleEmitter::getParticles()
{
	return m_particles;
}

void ParticleEmitter::setSpawnDelay(float spawnDelay)
{
	m_spawnDelay = spawnDelay;
}

float ParticleEmitter::getSpawnDelay()
{
	return m_spawnDelay;
}

void ParticleEmitter::updateParticles(float deltaTime)
{
	m_spawnTimer += deltaTime;
	if (m_spawnTimer >= m_spawnDelay && m_particles.size() < m_maxParticles)
	{
		m_spawnTimer = 0;

		Vector3 rotation = Vector3(randFloat(m_particleRotation.x - m_particleRotationVariation.x, m_particleRotation.x + m_particleRotationVariation.x), randFloat(m_particleRotation.y - m_particleRotationVariation.y, m_particleRotation.y + m_particleRotationVariation.y), randFloat(m_particleRotation.z - m_particleRotationVariation.z, m_particleRotation.z + m_particleRotationVariation.z));
		Vector3 scale = Vector3(randFloat(m_particleScale.x - m_particleScaleVariation.x, m_particleScale.x + m_particleScaleVariation.x), randFloat(m_particleScale.y - m_particleScaleVariation.y, m_particleScale.y + m_particleScaleVariation.y), randFloat(m_particleScale.z - m_particleScaleVariation.z, m_particleScale.z + m_particleScaleVariation.z));
		Vector3 force = Vector3(randFloat(m_force.x - m_forceRange.x, m_force.x + m_forceRange.x), randFloat(m_force.y - m_forceRange.y, m_force.y + m_forceRange.y), randFloat(m_force.z - m_forceRange.z, m_force.z + m_forceRange.z));
		float lifetime = randFloat(m_particleLifetime - m_particleLifetimeVariation, m_particleLifetime + m_particleLifetimeVariation);

		Particle* newParticle = new Particle(m_particleGeometry, m_position, rotation, scale, lifetime);
		newParticle->InitParticleData(m_device, m_particleTexturePath);
		newParticle->setForce(force);
		m_particles.push_back(newParticle);
	}

	for (int i = (int)m_particles.size() - 1; i >= 0; i--)
	{
		m_particles[i]->Update(deltaTime);

		if (!m_particles[i]->isAlive())
		{
			Particle* particleToErase = m_particles[i];
			m_particles.erase(m_particles.begin() + i);
			delete particleToErase;
		}
	}
}

void ParticleEmitter::drawParticles(ID3D11DeviceContext1* context,
									ConstantBuffer& cb,
									ID3D11Buffer* constantBuffer)
{
	for (Particle* particle : m_particles)
	{
		particle->Draw(context, cb, constantBuffer);
	}
}

float ParticleEmitter::randFloat(float low, float high)
{
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}
