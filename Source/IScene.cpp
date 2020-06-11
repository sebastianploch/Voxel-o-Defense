#include "pch.h"
#include "IScene.h"

using namespace DirectX;

IScene::IScene(SCENETYPE type,
			   SceneManager* sceneManager,
			   int windowWidth,
			   int windowHeight,
			   UIManager& uiManager,
			   CommonStates& states) :
	m_type(type),
	m_sceneManager(sceneManager),
	m_uiManager(uiManager),
	m_states(states),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight)
{
}