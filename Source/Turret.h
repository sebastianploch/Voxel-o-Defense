#pragma once
#pragma once
#include "ParticleEmitter.h"
#include "Model.h"
#include "AiAgent.h"
#include "Sound.h"

class Turret
{
public:
	Turret(DirectX::XMFLOAT3 pos);
	void Update(float deltaTime,std::vector<std::shared_ptr<AiAgent>> agents);

	inline void SetIsActive(bool active) { m_isActive = active; }
	inline bool GetIsActive() const { return m_isActive; }

	inline void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }

private:
	DirectX::XMFLOAT3 m_position;
	float m_fireRate;
	int m_damage;
	float m_radius;
	bool m_isActive = false;
};