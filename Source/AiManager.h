#pragma once
#ifndef AI_MANAGER
#define AI_MANAGER

//-----------//
//- Defines -//
//-----------//

#define NullType NULLTYPE, 100, 1, 1, false, 1 
#define Zombie ZOMBIE, 100, 1, 2, false, 1
#define Spider SPIDER, 125, 3, 1, false, 50
#define Skeleton SKELETON, 150, 2, 2, false, 1

//------------//
//- Includes -//
//------------//

#include <DirectXMath.h>
#include <vector>
#include "Node.h"
#include "AiAgent.h"
#include "RouteConstructor.h"

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

	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* deviceContext, ConstantBuffer cb, ID3D11Buffer* constBuffer);

protected:
	std::vector<std::shared_ptr<AiAgent>> GetAiAgents();
	int GetTotalAgents();
	void SetTotalAgents(int totalAgents);

private:
	std::vector<std::shared_ptr<AiAgent>> m_aiAgents;
	int m_totalAmountOfAgents;
	DirectX::XMFLOAT3 m_spawnRates;
	std::shared_ptr<RouteConstructor> m_routeConstructor;

};
#endif // !AI_MANAGER