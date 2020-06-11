#include "pch.h"
#include "AiManager.h"
#include "ChunkHandler.h"
#include <thread>

// Ignore 'unscoped enum' warning
#pragma warning(disable : 4458)

AiManager::AiManager(int totalAgents, DirectX::XMFLOAT3 spawnRate,EnemyFactory* factory)
{
	m_totalAmountOfAgents = totalAgents;
	m_spawnRates = spawnRate;

	for (int i = 0; i < m_totalAmountOfAgents; i++)
	{
		int MonsterSelection = rand() % 100;
		if (MonsterSelection <= m_spawnRates.x) 
		{
			std::shared_ptr<AiAgent> temp = std::make_shared<AiAgent>(Zombie, *factory);
			m_aiAgents.push_back(temp);
		}
		else if (MonsterSelection <= m_spawnRates.x + m_spawnRates.y) 
		{
			std::shared_ptr<AiAgent> temp = std::make_shared<AiAgent>(Spider, *factory);
			m_aiAgents.push_back(temp);
		}
		else if (MonsterSelection <= m_spawnRates.x + m_spawnRates.y + m_spawnRates.z)
		{
			std::shared_ptr<AiAgent> temp = std::make_shared<AiAgent>(Skeleton, *factory);
			m_aiAgents.push_back(temp);
		}
		else
		{
			std::shared_ptr<AiAgent> temp = std::make_shared<AiAgent>(Zombie, *factory); 
			m_aiAgents.push_back(temp);
		}
	}

	m_routeConstructor = std::make_shared<RouteConstructor>();
	ImportTerrainInfo();

	SetStartLocation(DirectX::XMFLOAT3(0, 4, 0), 0);
	SetEndLocation(DirectX::XMFLOAT3(((32 * 15) / 2), 4, ((32 * 15) / 2)), 0);

	SetStartLocation(DirectX::XMFLOAT3(480, 4, 0), 1);
	SetEndLocation(DirectX::XMFLOAT3(((32 * 15) / 2), 4, ((32 * 15) / 2)), 1);

	SetStartLocation(DirectX::XMFLOAT3(0, 4, 480), 2);
	SetEndLocation(DirectX::XMFLOAT3(((32 * 15) / 2), 4, ((32 * 15) / 2)), 2);

	SetStartLocation(DirectX::XMFLOAT3(480, 4, 480), 3);
	SetEndLocation(DirectX::XMFLOAT3(((32 * 15) / 2), 4, ((32 * 15) / 2)), 3);
}

AiManager::~AiManager()
{
}

void AiManager::Update(float deltaTime, float time)
{
	if (CalculationsDone == true)
	{
		for (int i = 0; i < m_aiAgents.size(); i++)
		{
			m_aiAgents[i]->Update(deltaTime, time);
		}
	}
}

void AiManager::Render(ID3D11DeviceContext* deviceContext, ID3D11DeviceContext1* dc1, ConstantBuffer cb, ID3D11Buffer* constBuffer, ShaderManager* m_shaderManager)
{
	for (int i = 0; i < m_aiAgents.size(); i++)
	{
		// Assign Shader to be used to render upcoming object
		m_shaderManager->SetShader(m_aiAgents[i]->GetGameObject()->GetShaderType(), dc1);

		// Assign Object World Mat data to ConstantBuffer
		cb.world = m_aiAgents[i]->GetGameObject()->GetWorldMatrix();

		deviceContext->UpdateSubresource(constBuffer,
			0,
			nullptr,
			&cb,
			0, 0);

		m_aiAgents[i]->Render(constBuffer,cb,dc1);
	}
}

void AiManager::ImportTerrainInfo()
{
	m_routeConstructor->CreatePathfindingMap();
}

void AiManager::SetStartLocation(DirectX::XMFLOAT3 pos, int startingPosition)
{
	m_routeConstructor->SetStarting(pos, startingPosition);
}

void AiManager::SetEndLocation(DirectX::XMFLOAT3 pos, int startingPosition)
{
	m_routeConstructor->SetEnding(pos, startingPosition);
}

void AiManager::StartWave()
{  
	CalculationsDone = false;

	for (int i = 0; i < m_aiAgents.size(); i++)
	{
		switch (m_aiAgents[i]->GetType())
		{
		case ZOMBIE:
			m_aiAgents[i]->ResetAgent(Zombie);
			break;
		case SPIDER:
			m_aiAgents[i]->ResetAgent(Spider);
			break;
		case SKELETON:
			m_aiAgents[i]->ResetAgent(Skeleton);
			break;
		default:
			m_aiAgents[i]->ResetAgent(Zombie);
			break;
		}
	}

	m_routeConstructor->UpdatePathfindingMap();

	SetStartLocation(DirectX::XMFLOAT3(0, 4, 0), 0);
	SetEndLocation(DirectX::XMFLOAT3(((32 * 15) / 2), 4, ((32 * 15) / 2)), 0);

	SetStartLocation(DirectX::XMFLOAT3(480, 4, 0), 1);
	SetEndLocation(DirectX::XMFLOAT3(((32 * 15) / 2), 4, ((32 * 15) / 2)), 1);

	SetStartLocation(DirectX::XMFLOAT3(0, 4, 480), 2);
	SetEndLocation(DirectX::XMFLOAT3(((32 * 15) / 2), 4, ((32 * 15) / 2)), 2);

	SetStartLocation(DirectX::XMFLOAT3(480, 4, 480), 3);
	SetEndLocation(DirectX::XMFLOAT3(((32 * 15) / 2), 4, ((32 * 15) / 2)), 3);

	m_routeConstructor->A_star(0);
	m_routeConstructor->A_star(1);
	m_routeConstructor->A_star(2);
	m_routeConstructor->A_star(3);

	int RouteSelector = 0;

	for (int i = 0; i < m_aiAgents.size(); i++)
	{
		m_aiAgents[i]->SetRoute(m_routeConstructor->GetRoute(RouteSelector, m_aiAgents[i]->GetStepHeight()));
		m_aiAgents[i]->SpawnAiAgent(i * 5);

		if (RouteSelector == 3)
		{
			RouteSelector = 0;
		}
		else
		{
			RouteSelector++;
		}
	}

	CalculationsDone = true;
}

bool AiManager::HasWaveEnded()
{
	for (int i = 0; i < m_aiAgents.size(); i++)
	{
		if (m_aiAgents[i]->IsAiAgentActive() == true)
		{
			return false;
		}
	}
	return true;
}

//---------------------------------------------------//
//------------ Access Variable Functions ------------//
//---------------------------------------------------//

std::vector<std::shared_ptr<AiAgent>> AiManager::GetAiAgents()
{
	return m_aiAgents;
}

int AiManager::GetTotalAgents()
{
	return 0;
}

void AiManager::SetTotalAgents(int totalAgents)
{
	m_totalAmountOfAgents = totalAgents;
}
