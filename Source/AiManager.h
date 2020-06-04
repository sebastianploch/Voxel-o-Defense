#pragma once
#ifndef AI_MANAGER
#define AI_MANAGER

//-----------//
//- Defines -//
//-----------//

// type health movementSPeed damage active stepup
#define NullType NULLTYPE, 0.1, 1, 1, false, 1
#define Zombie ZOMBIE, 1, 1, 2, false, 1
#define Spider SPIDER, 0.1, 3, 1, false, 50
#define Skeleton SKELETON, 0.1, 2, 2, false, 1

//------------//
//- Includes -//
//------------//

#include <DirectXMath.h>
#include <vector>
#include "Node.h"
#include "AiAgent.h"
#include "RouteConstructor.h"
#include "ShaderManager.h"

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
	AiManager(int totalAgents, DirectX::XMFLOAT3 SpawnRate);
	~AiManager();

	void Update(float deltaTime, float time);
	void Render(ID3D11DeviceContext* deviceContext, ID3D11DeviceContext1* dc1, ConstantBuffer cb, ID3D11Buffer* constBuffer, ShaderManager* m_shaderManager);

	void ImportTerrainInfo();
	void SetStartLocation(DirectX::XMFLOAT3 pos, int startingPosition);
	void SetEndLocation(DirectX::XMFLOAT3 pos, int startingPosition);

	std::vector<std::shared_ptr<AiAgent>> GetAiAgents();
	int GetTotalAgents();
	void SetTotalAgents(int totalAgents);

	void StartWave(STEP_UP_AMOUNT step, int startingLocation);
	bool HasWaveStarted();

private:
	std::vector<std::shared_ptr<AiAgent>> m_aiAgents;
	int m_totalAmountOfAgents;
	DirectX::XMFLOAT3 m_spawnRates;
	std::shared_ptr<RouteConstructor> m_routeConstructor;
};
#endif // !AI_MANAGER