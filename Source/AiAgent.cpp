#include "pch.h"
#include "AiAgent.h"
#include <DirectXMath.h>
#include <cmath>

#pragma region Constructor_Destructor
AiAgent::AiAgent(TypeOfMonster type, int health, float movementSpeed, float damage, bool active, int maxStepUp)
{
	_type = type;
	_health = health;
	_movementSpeed = movementSpeed;
	_damage = damage;
	_active = active;
	_stepHeightIndex = maxStepUp;

	m_position = DirectX::SimpleMath::Vector3(0.5f, 4.5f, 0.5f);
	m_gameObject = std::make_unique<DumbObject>(m_position, DirectX::SimpleMath::Vector3(), DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f), "Resources/config/cube.json", "cube");
}

AiAgent::~AiAgent()
{
	
}
#pragma endregion Constructor_Destructor

#pragma region Game_Loops
void AiAgent::Update(float deltaTime, float time)
{
	if (_active == true)
	{
		if (_SpawnOffset == 0)
		{
			if (_totalTimeCoverd > 1.0f)
			{
				_totalTimeCoverd = 0.0f;
				m_route.erase(m_route.end() - 1);
				if (m_route.size() != 0)
				{
					m_position = m_route[m_route.size() - 1]->GetPosition() + DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f);
					m_gameObject->SetPosition(m_position);
				}
				else
				{
					_active = false;
				}
			}
			else
			{
				_totalTimeCoverd += _movementSpeed;
			}
		}
		else
		{
			_SpawnOffset--;
		}
	}

	m_gameObject->Update(deltaTime);
}

void AiAgent::Render(ID3D11DeviceContext* deviceContex)
{
	if (_active == true && _SpawnOffset == 0)
	{
		m_gameObject->Draw(deviceContex);
	}
}
#pragma endregion Game_Loop

#pragma region Calculations
float AiAgent::ReturnDistance(DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2)
{
	return ((abs(v1.x - v2.x) + abs(v1.y - v2.y) + abs(v1.z - v2.z)) / 2);
}

DirectX::SimpleMath::Vector3 AiAgent::Lerp(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, float t)
{
	return (start + t * (end - start));
}
#pragma endregion Calculations

#pragma region Setting_Getting_Functions

void AiAgent::SetRoute(std::vector<Nodes*> route)
{
	m_route = route;
}

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

int AiAgent::GetStepHeight()
{
	return _stepHeightIndex;
}

bool AiAgent::IsAiAgentActive()
{
	return _active;
}

void AiAgent::SpawnAiAgent(int offset)
{
	_SpawnOffset = offset;
	_active = true;
}

void AiAgent::DieAiAgent()
{
	_active = false;
}

DirectX::SimpleMath::Vector3 AiAgent::GetPosition()
{
	return m_position;
}

bool AiAgent::IsAgentCompleatlyStopped()
{
	return m_compleateStop;
}

bool AiAgent::HasFinalDestinationChanged()
{
	return m_endingDestinationChanged; 
}
#pragma endregion Setting_Getting_Functions