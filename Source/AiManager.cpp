#include "pch.h"
#include "AiManager.h"

AiManager::AiManager(int totalAgents, DirectX::XMFLOAT3 spawnRate)
{
	m_totalAmountOfAgents = totalAgents;
	m_spawnRates = spawnRate;

	for (int i = 0; i < m_totalAmountOfAgents; i++)
	{
		float ten = rand() % 100;

		m_aiAgents.push_back(std::make_shared<AiAgent>(Zombie));
	}
	m_routeConstructor = std::make_shared<RouteConstructor>();
}

AiManager::~AiManager()
{
}

void AiManager::Update(float deltaTime)
{
	for (int i = 0; i < m_aiAgents.size(); i++)
	{
		m_aiAgents[i]->Update(deltaTime);
	}
}

void AiManager::Render(ID3D11DeviceContext* deviceContext, ConstantBuffer cb, ID3D11Buffer* constBuffer)
{
	for (int i = 0; i < m_aiAgents.size(); i++)
	{
		deviceContext->UpdateSubresource(constBuffer,
			0,
			nullptr,
			&cb,
			0, 0);

		m_aiAgents[i]->Render(deviceContext);
	}
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
