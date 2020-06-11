#pragma once

#include "IScene.h"

class UIManager;

class MenuScene :
	public IScene
{
public:
	MenuScene(SceneManager* sceneManager,
			  UIManager& uiManager,
			  DirectX::CommonStates& states);
	~MenuScene();

	void OnNotify(Event* event) override;

	void Initialise(int windowWidth,
					int windowHeight,
					UIManager& uiManager,
					ID3D11Device* device, 
					CameraManager* manager);

	void Render(ID3D11DeviceContext1* context, ConstantBuffer& cb, ID3D11Buffer* constantBuffer, DirectX::SpriteBatch* spriteBatch) override;
	bool Update(float deltaTime, float time, InputState& inputState) override;

private:
	int m_uiID[3];
	CameraManager* m_cameraManager;
};