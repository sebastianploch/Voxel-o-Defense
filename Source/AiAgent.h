#pragma once
#ifndef AI_AGENT
#define AI_AGENT

//-----------//
//- Defines -//
//-----------//



//------------//
//- Includes -//
//------------//

#include "DumbObject.h"
#include "Node.h"

//-----------//
//-- Enums --//
//-----------//



//------------//
//-- Struct --//
//------------//

enum TypeOfMonster
{
	NULLTYPE = 0,
	ZOMBIE,
	SPIDER,
	SKELETON
};

//-----------//
//-- Class --//
//-----------//
class AiAgent
{
public:
	AiAgent(TypeOfMonster type, int health, float movementSpeed, float damage, bool active, int maxStepUp);
	~AiAgent();

	void Update(float deltaTime, float time);
	float ReturnDistance(DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2);

	DirectX::SimpleMath::Vector3 Lerp(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, float t);

	void Render(ID3D11DeviceContext* deviceContex);

	void SetRoute(std::vector<Nodes*> route);

	int GetHealth();
	void SetHealth(int passIn);

	float GetMovementSpeed();
	void SetMovementSpeed(float passIn);

	int GetDamage();
	void SetDamage(int passIn);

	TypeOfMonster GetType();
	void SetType(TypeOfMonster type, int health, float movementSpeed, float damage, bool active, int maxStepUp);

	std::shared_ptr<DumbObject> GetGameObject();

	std::vector<Nodes*> GetRoute();

	int GetStepHeight();

	bool IsAiAgentActive();
	void SpawnAiAgent(int offSet);
	void DieAiAgent();

	DirectX::SimpleMath::Vector3 GetPosition();
	bool IsAgentCompleatlyStopped();
	bool HasFinalDestinationChanged();

private:
	//- In Game Object -//
	std::shared_ptr<DumbObject> m_gameObject;
	
	DirectX::SimpleMath::Vector3 m_position;
	bool m_compleateStop;
	bool m_endingDestinationChanged;

	//- Route The Ai Agent Will Travel Along -//
	std::vector<Nodes*> m_route;

	DirectX::SimpleMath::Vector3 CurrentNodePosition, NextNodePosition;

	TypeOfMonster _type = NULLTYPE;
	int _health = 0;
	float _movementSpeed = 0.0f;
	int _damage = 0;
	bool _active = false;
	int _stepHeightIndex = 0;
	
	float _startTime = 0.0f;
	float _totalTimeCoverd = 0;
	int _SpawnOffset = 0;
};
#endif // !AI_AGENT
