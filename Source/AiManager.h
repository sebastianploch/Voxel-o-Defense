#pragma once
#ifndef AI_MANAGER
#define AI_MANAGER

//-----------//
//- Defines -//
//-----------//

// movementSpeed lower is slower
// Type Health MovementSpeed Damage Active Stepup
#define NullType	NULLTYPE,	10, 1, 2, false, ONE
#define Zombie		ZOMBIE,		10, 1, 2, false, ONE
#define Spider		SPIDER,		10, 1, 2, false, FIVE
#define Skeleton	SKELETON,	10, 1, 2, false, ONE

//------------//
//- Includes -//
//------------//

#include <DirectXMath.h>
#include <vector>
#include "Node.h"
#include "AiAgent.h"
#include "RouteConstructor.h"
#include "ShaderManager.h"
#include "Enemy.h"

//-----------//
//-- Enums --//
//-----------//



//------------//
//-- Struct --//
//------------//



//-----------//
//-- Class --//
//-----------//
class  AiManager
{
public:
	AiManager(int totalAgents, DirectX::XMFLOAT3 SpawnRate,EnemyFactory *factory);
	~AiManager();

	void Update(float deltaTime, float time);
	void Render(ID3D11DeviceContext* deviceContext, ID3D11DeviceContext1* dc1, ConstantBuffer cb, ID3D11Buffer* constBuffer, ShaderManager* m_shaderManager);

	void ImportTerrainInfo();
	void SetStartLocation(DirectX::XMFLOAT3 pos, int startingPosition);
	void SetEndLocation(DirectX::XMFLOAT3 pos, int startingPosition);

	std::vector<std::shared_ptr<AiAgent>> GetAiAgents();
	int GetTotalAgents();
	void SetTotalAgents(int totalAgents);

	void StartWave();
	bool HasWaveEnded();

private:
	std::vector<std::shared_ptr<AiAgent>> m_aiAgents;
	int m_totalAmountOfAgents;
	DirectX::XMFLOAT3 m_spawnRates;
	std::shared_ptr<RouteConstructor> m_routeConstructor;

	bool CalculationsDone = false;
	int temp = 0;
};
#endif // !AI_MANAGER