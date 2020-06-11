#pragma once

#include "IScene.h"

class UIManager;

class SplashScene :
	public IScene
{
public:
	SplashScene(SceneManager* sceneManager,
			  UIManager& uiManager,
			  DirectX::CommonStates& states);
	~SplashScene();

	void OnNotify(Event* event) override;

	void Initialise(int windowWidth,
					int windowHeight,
					UIManager& uiManager,
					ID3D11Device* device);
	int CreateText(const wchar_t* name, DirectX::SimpleMath::Vector2 pos, UIManager& uiManager, ID3D11Device* device); // Creates a UIText and returns the ID

	void Render(ID3D11DeviceContext1* context, ConstantBuffer& cb, ID3D11Buffer* constantBuffer, DirectX::SpriteBatch* spriteBatch) override;
	bool Update(float deltaTime, float time, InputState& inputState) override;

private:
	int m_uiID[8];

	const float m_lifeTime = 4; // Keeps track of scene lifetime, in seconds
	float m_lifeTimeTimer; // Counts down from scene lifetime, scene is removed once this reaches 0
};