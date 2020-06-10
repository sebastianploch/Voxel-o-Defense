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
	if (this->m_fireRate <= 0)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			DirectX::XMFLOAT3 pos = agents[i]->GetGameObject()->GetTranslation();
			float distance =
				sqrt(pow(m_position.x - pos.x, 2) +
					pow(m_position.y - pos.y, 2) +
					pow(m_position.z - pos.z, 2));
			if (distance < m_radius && agents[i]->GetHealth() > 0)
			{
				agents[i]->SetHealth(agents[i]->GetHealth() - m_damage);
				Sound::Fire(L"PlaceStructure");
				DEBUG_PRINT(DirectX::SimpleMath::Vector3(agents[i]->GetType(),distance, agents[i]->GetHealth()));
				break;
			}
		}
		this->m_fireRate = 0.1f;
	}
	this->m_fireRate -= deltaTime;
}