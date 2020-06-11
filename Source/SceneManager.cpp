#include "pch.h"
#include "SceneManager.h"

#include "IScene.h"
#include "MenuScene.h"
#include "PlayScene.h"
#include "SplashScene.h"

using namespace DirectX;

SceneManager::SceneManager(int windowWidth,
						   int windowHeight,
						   Microsoft::WRL::ComPtr <ID3D11Device1> device,
						   UIManager& uiManager,
						   CameraManager& cameraManager,
						   ShaderManager& shaderManager,
						   DirectX::CommonStates& states) :
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_d3dDevice(device),
	m_UIManager(uiManager),
	m_cameraManager(cameraManager),
	m_shaderManager(shaderManager),
	m_states(states)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update(float deltaTime, float time, InputState& inputState)
{
	//int count = 0;
	//for (int i = 0; i < m_scenes.size(); i++)
	//{
	//	if (m_scenes[i])
	//		if (m_scenes[i]->Update(deltaTime, time, inputState))
	//			Remove(m_scenes[i]->GetType());
	//	count++;
	//}

	for (auto& scene : m_scenes)
	{
		if (scene)
			if (scene->Update(deltaTime, time, inputState))
				Remove(scene->GetType());
	}
}

void SceneManager::Render(ID3D11DeviceContext1* context, ConstantBuffer& cb, ID3D11Buffer* constantBuffer, DirectX::SpriteBatch* spriteBatch)
{
	for (const auto& scene : m_scenes)
	{
		if (scene)
			scene->Render(context, cb, constantBuffer, spriteBatch);
	}
}

void SceneManager::SetWindowDimensions(int windowWidth, int windowHeight)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	for (auto& scene : m_scenes)
	{
		if (scene)
			scene->SetWindowDimensions(windowWidth, windowHeight);
	}
}

void SceneManager::Add(SCENETYPE sceneType)
{
	if (!Find(sceneType))
	{
		switch (sceneType)
		{
		case SCENETYPE::MENU:
				MenuScene* menuScene;
				menuScene = new MenuScene(this, m_UIManager, m_states);
				menuScene->Initialise(m_windowWidth, m_windowHeight, m_UIManager, m_d3dDevice.Get());

				m_scenes.emplace_back(std::unique_ptr<MenuScene>(menuScene));
			break;
		case SCENETYPE::PLAY:
				PlayScene* playScene;
				playScene = new PlayScene(this, m_windowWidth, m_windowHeight, m_UIManager, m_d3dDevice, m_cameraManager, m_shaderManager, m_states);

				m_scenes.emplace_back(std::unique_ptr<PlayScene>(playScene));
			break;
		case SCENETYPE::SPLASH:
				SplashScene* splashScene;
				splashScene = new SplashScene(this, m_UIManager, m_states);
				splashScene->Initialise(m_windowWidth, m_windowHeight, m_UIManager, m_d3dDevice.Get());

				m_scenes.emplace_back(std::unique_ptr<SplashScene>(splashScene));
			break;
		}
	}
}

void SceneManager::Remove(SCENETYPE sceneType)
{
	for (int i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i])
			if (m_scenes[i]->GetType() == sceneType)
			{
				m_scenes[i].reset();
			}
	}
}

bool SceneManager::Find(SCENETYPE sceneType)
{
	for (auto& scene : m_scenes)
		if (scene && scene->GetType() == sceneType)
			return true;

	return false;
}