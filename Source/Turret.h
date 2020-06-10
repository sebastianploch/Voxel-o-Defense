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
private:
	DirectX::XMFLOAT3 m_position;
	float m_fireRate;
	int m_damage;
	float m_radius;
protected:
};