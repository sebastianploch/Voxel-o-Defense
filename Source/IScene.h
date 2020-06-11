#pragma once

#include "Observer.h"
#include "SceneManager.h"

class SceneManager;

class IScene :
	public Observer
{
public:
	IScene(SCENETYPE type,
		   SceneManager* sceneManager,
		   int windowWidth,
		   int windowHeight,
		   UIManager& uiManager,
		   DirectX::CommonStates& states);
	virtual ~IScene() {}

	virtual void Render(ID3D11DeviceContext1* context, ConstantBuffer& cb, ID3D11Buffer* constantBuffer, DirectX::SpriteBatch* spriteBatch) = 0;
	virtual bool Update(float deltaTime, float time, InputState& inputState) = 0;

	void SetWindowDimensions(int windowWidth, int windowHeight) { m_windowWidth = windowWidth; m_windowHeight = windowHeight; }

	SCENETYPE GetType() { return m_type; }

protected:
	SCENETYPE				m_type;
	SceneManager*			m_sceneManager;
	int						m_windowWidth;
	int						m_windowHeight;

	// Refs
	UIManager&				m_uiManager;
	DirectX::CommonStates&  m_states;
};