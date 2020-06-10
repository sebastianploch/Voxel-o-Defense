#pragma once
#include "pch.h"
#include "Turret.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "pch.h"

Turret::Turret(DirectX::XMFLOAT3 pos)
{
	this->m_position = pos;
	this->m_fireRate = 0.1f;
	this->m_radius = 45.0f;
	this->m_damage = 2;
}

void Turret::Update(float deltaTime, std::vector<std::shared_ptr<AiAgent>> agents)
{
	if (!m_isActive)
		return;

	if (this->m_fireRate <= 0)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			DirectX::XMFLOAT3 enemyPos = agents[i]->GetGameObject()->GetTranslation();
			float distance =
				sqrt(pow(m_position.x - enemyPos.x, 2) +
					pow(m_position.y - enemyPos.y, 2) +
					pow(m_position.z - enemyPos.z, 2));
			if (distance < m_radius && agents[i]->GetHealth() > 0)
			{
				agents[i]->SetHealth(agents[i]->GetHealth() - m_damage);
				Sound::Fire(L"TurretTier1Shoot");
				break;
			}
		}
		this->m_fireRate = 0.1f;
	}
	this->m_fireRate -= deltaTime;
}