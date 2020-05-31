#include "pch.h"
#include "AiAgent.h"
#include <DirectXMath.h>

AiAgent::AiAgent(TypeOfMonster type, int health, float movementSpeed, float damage, bool active, int maxStepUp)
{
	_type = type;
	_health = health;
	_movementSpeed = movementSpeed;
	_damage = damage;
	_active = active;

	m_gameObject = std::make_unique<DumbObject>(DirectX::SimpleMath::Vector3(0.25f, 2.25f, 0.25f), DirectX::SimpleMath::Vector3(), DirectX::SimpleMath::Vector3(0.25f, 0.25f, 0.25f), "Resources/config/cube.json", "cube");

}

AiAgent::~AiAgent()
{
	
}

void AiAgent::Update(float deltaTime)
{
	m_gameObject->Update(deltaTime);
}

void AiAgent::Render(ID3D11DeviceContext* deviceContex)
{
	m_gameObject->Draw(deviceContex);
}

//---------------------------------------------------//
//------------ Access Variable Functions ------------//
//---------------------------------------------------//

int AiAgent::GetHealth()
{
	return _health;
}

void AiAgent::SetHealth(int passIn)
{
	_health = passIn;
}

float AiAgent::GetMovementSpeed()
{
	return _movementSpeed;
}

void AiAgent::SetMovementSpeed(float passIn)
{
	_movementSpeed = passIn;
}

int AiAgent::GetDamage()
{
	return _damage;
}

void AiAgent::SetDamage(int passIn)
{
	_damage = passIn;
}

TypeOfMonster AiAgent::GetType()
{
	return _type;
}

void AiAgent::SetType(TypeOfMonster type, int health, float movementSpeed, float damage, bool active, int maxStepUp)
{
	_type = type;
	_health = health;
	_movementSpeed = movementSpeed;
	_damage = damage;
	_active = active;
}

std::shared_ptr<DumbObject> AiAgent::GetGameObject()
{
	return m_gameObject;
}

std::vector<Nodes*> AiAgent::GetRoute()
{
	return m_route;
}

bool AiAgent::IsAiAgentActive()
{
	return _active;
}

void AiAgent::SpawnAiAgent()
{
	_active = true;
}

void AiAgent::DieAiAgent()
{
	_active = false;
}

DirectX::XMFLOAT3 AiAgent::GetPosition()
{
	return *m_positionRef;
}

bool AiAgent::IsAgentCompleatlyStopped()
{
	return m_compleateStop;
}

bool AiAgent::HasFinalDestinationChanged()
{
	return m_endingDestinationChanged; 
}