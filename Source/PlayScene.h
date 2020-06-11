#pragma once

#include "IScene.h"

#include "AiManager.h"
#include "BuildManager.h"
#include "Enemy.h"
#include "Subject.h"
#include "Turret.h"

class UIManager;

class PlayScene :
	public IScene
{
public:
	PlayScene(SceneManager* sceneManager,
			  int windowWidth,
			  int windowHeight,
			  UIManager& uiManager,
			  Microsoft::WRL::ComPtr <ID3D11Device1> device,
			  CameraManager& cameraManager,
			  ShaderManager& shaderManager,
			  DirectX::CommonStates& states);
	~PlayScene();

	void OnNotify(Event* event) override;

	void Render(ID3D11DeviceContext1* context, ConstantBuffer& cb, ID3D11Buffer* constantBuffer, DirectX::SpriteBatch* spriteBatch) override;
	bool Update(float deltaTime, float time, InputState& inputState) override;

private:
	void InitialiseVoxelWorld();
	void InitialiseBuildModeUI();

private:
	// Refs
	ID3D11Device1*		  m_d3dDevice;
	CameraManager&		  m_cameraManager;
	ShaderManager&		  m_shaderManager;

	// Game Objects
	std::vector<std::shared_ptr<IGameObject>>	m_gameObjects;

	// Ai Manager
	std::shared_ptr<AiManager>					m_AiManager;

	std::shared_ptr<EnemyFactory>               m_enemyFactory;

	// Building
	std::unique_ptr<BuildManager>				m_buildManager;
	std::vector<int>                            m_buildModeIDs;	// Store Build Mode UI ID's

	// Turret Object
	std::vector<std::shared_ptr<Turret>>		m_turrets = std::vector<std::shared_ptr<Turret>>();
};