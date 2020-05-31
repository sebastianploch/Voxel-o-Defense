#include "pch.h"
#include "AiManager.h"
#include "ChunkHandler.h"

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
	ImportTerrainInfo();
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

		m_aiAgents[i]->Render(deviceContext);
	}
}

void AiManager::ImportTerrainInfo()
{
	m_routeConstructor->CreatePathfindingMap();
}

void AiManager::SetStartLocation(DirectX::XMFLOAT3 pos)
{
	m_routeConstructor->SetStarting(pos);
}

void AiManager::SetEndLocation(DirectX::XMFLOAT3 pos)
{
	m_routeConstructor->SetEnding(pos);
}

void AiManager::StartWave()
{
	m_routeConstructor->A_star();
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
