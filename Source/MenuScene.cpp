#include "pch.h"
#include "MenuScene.h"

#include "ISOCamera.h"

#include "UIButton.h"
#include "UISprite.h"
#include "UIManager.h"

using namespace DirectX;

MenuScene::MenuScene(SceneManager* sceneManager,
					 UIManager& uiManager,
					 CommonStates& states) :
	IScene(SCENETYPE::MENU, sceneManager, 0, 0, uiManager, states)
{
}

MenuScene::~MenuScene()
{
	for (int i = 0; i < sizeof(m_uiID); i++)
		m_uiManager.Remove(m_uiID[i]);
}

void MenuScene::OnNotify(Event* event)
{
	switch (event->m_type)
	{
	case EVENTTYPE::CLICK:
		ISOCamera* cam = static_cast<ISOCamera*>(m_cameraManager->GetActiveCamera());
		cam->SetIsBuildMode(true);
		m_sceneManager->Remove(SCENETYPE::MENU);
		break;
	}
}

void MenuScene::Initialise(int windowWidth,
						   int windowHeight,
						   UIManager& uiManager,
						   ID3D11Device* device, 
						   CameraManager* cameraManager)
{
	m_cameraManager = cameraManager;

	// Create background image
	std::shared_ptr<UISprite> backgroundImage = std::make_shared<UISprite>();
	backgroundImage->Initialise(SimpleMath::Vector2(0.0f, 0.0f), L"Resources/Textures/MainMenuOverlay.dds", device, -1.0f, windowWidth, windowHeight);
	m_uiID[0] = uiManager.Add(backgroundImage);

	// Create logo image
	std::shared_ptr<UISprite> logoImage = std::make_shared<UISprite>();
	logoImage->Initialise(SimpleMath::Vector2((float)windowWidth / 2.0f, (float)windowHeight / 4.0f), L"Resources/Textures/Logo.dds", device);
	m_uiID[1] = uiManager.Add(logoImage);
	

	// Create start button
	std::shared_ptr<UIButton> startButton = std::make_shared<UIButton>();
	startButton->Initialise(SimpleMath::Vector2((float)windowWidth / 2.0f, (float)windowHeight * 0.7f), L"Resources/Textures/RegularButton.dds", L"Resources/Fonts/5x5.spritefont", L"Start", device);
	m_uiID[2] = uiManager.Add(startButton);
	startButton->Clicked()->AddObserver(this);
}

bool MenuScene::Update(const float deltaTime, float time, InputState& inputState)
{
	UNREFERENCED_PARAMETER(deltaTime);
	UNREFERENCED_PARAMETER(time);
	UNREFERENCED_PARAMETER(inputState);

	return false;
}

void MenuScene::Render(ID3D11DeviceContext1* context, ConstantBuffer& cb, ID3D11Buffer* constantBuffer, DirectX::SpriteBatch* spriteBatch)
{
	UNREFERENCED_PARAMETER(cb);
	UNREFERENCED_PARAMETER(constantBuffer);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(spriteBatch);

	//spriteBatch->Begin(SpriteSortMode_Deferred, nullptr, m_states.PointClamp());
	//m_uiManager.Render(spriteBatch);
	//spriteBatch->End();
} 