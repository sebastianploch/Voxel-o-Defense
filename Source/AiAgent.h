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

	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* deviceContex);

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

	bool IsAiAgentActive();
	void SpawnAiAgent();
	void DieAiAgent();

	DirectX::XMFLOAT3 GetPosition();
	bool IsAgentCompleatlyStopped();
	bool HasFinalDestinationChanged();

private:
	//- In Game Object -//
	std::shared_ptr<DumbObject> m_gameObject;
	
	DirectX::XMFLOAT3* m_positionRef;
	bool m_compleateStop;
	bool m_endingDestinationChanged;

	//- Route The Ai Agent Will Travel Along -//
	std::vector<Nodes*> m_route;

	TypeOfMonster _type = NULLTYPE;
	int _health = 0;
	float _movementSpeed = 0.0f;
	int _damage = 0;
	bool _active = false;
};
#endif // !AI_AGENT
