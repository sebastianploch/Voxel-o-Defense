#include "pch.h"
#include "SplashScene.h"

#include "UIButton.h"
#include "UISprite.h"
#include "UIText.h"
#include "UIManager.h"

using namespace DirectX;

SplashScene::SplashScene(SceneManager* sceneManager,
					 UIManager& uiManager,
					 CommonStates& states) :
	IScene(SCENETYPE::SPLASH, sceneManager, 0, 0, uiManager, states),
	m_lifeTimeTimer(m_lifeTime)
{
}

SplashScene::~SplashScene()
{
	for (int i = 1; i < sizeof(m_uiID); i++)
		m_uiManager.Remove(m_uiID[i]);

	m_sceneManager->Add(SCENETYPE::MENU);
}

void SplashScene::OnNotify(Event* event)
{
	UNREFERENCED_PARAMETER(event);
}

void SplashScene::Initialise(int windowWidth,
						   int windowHeight,
						   UIManager& uiManager,
						   ID3D11Device* device)
{
	// Create background image
	std::shared_ptr<UISprite> backgroundImage = std::make_shared<UISprite>();
	backgroundImage->Initialise(SimpleMath::Vector2(0.0f, 0.0f), L"Resources/Textures/SplashBG.dds", device, -1.0f, windowWidth, windowHeight);
	m_uiID[0] = uiManager.Add(backgroundImage);
	

	// Create texts
	SimpleMath::Vector2 pos = SimpleMath::Vector2((float)windowWidth / 2.0f, 0.0f);
	SimpleMath::Vector2 posAdd = SimpleMath::Vector2(0.0f, (float)windowHeight * 0.1f);

	m_uiID[1] = CreateText(L"MADE BY TEAM 2", pos + posAdd * 1, uiManager, device);

	m_uiID[2] = CreateText(L"Anthony Sturdy", pos + posAdd * 3, uiManager, device);
	m_uiID[3] = CreateText(L"David Taylor", pos+ posAdd * 4, uiManager, device);
	m_uiID[4] = CreateText(L"Joe Pickering", pos + posAdd * 5, uiManager, device);
	m_uiID[5] = CreateText(L"Krystal Hallett", pos + posAdd * 6, uiManager, device);
	m_uiID[6] = CreateText(L"Sebastian Ploch", pos + posAdd * 7, uiManager, device);
	m_uiID[7] = CreateText(L"Tom Silver", pos + posAdd * 8, uiManager, device);
}

int SplashScene::CreateText(const wchar_t* name, DirectX::SimpleMath::Vector2 pos, UIManager& uiManager, ID3D11Device* device)
{
	std::shared_ptr<UIText> nameText = std::make_shared<UIText>();
	nameText->Initialise(pos, name, SimpleMath::Color(0.0f, 0.0f, 0.0f), L"Resources/Fonts/Calibri.spritefont", device);
	return uiManager.Add(nameText);
}

bool SplashScene::Update(const float deltaTime, float time, InputState& inputState)
{
	UNREFERENCED_PARAMETER(time);

	Mouse::State mouseState = inputState.GetMouse().GetState();

	if (mouseState.leftButton)
	{
		//m_sceneManager->Remove(SCENETYPE::SPLASH);
		return true;
	}

	m_lifeTimeTimer -= deltaTime;

	if (m_lifeTimeTimer <= 0.0f)
	{
		//m_sceneManager->Remove(SCENETYPE::SPLASH);
		return true;
	}

	return false;
}

void SplashScene::Render(ID3D11DeviceContext1* context, ConstantBuffer& cb, ID3D11Buffer* constantBuffer, DirectX::SpriteBatch* spriteBatch)
{
	UNREFERENCED_PARAMETER(cb);
	UNREFERENCED_PARAMETER(constantBuffer);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(spriteBatch);

	//spriteBatch->Begin(SpriteSortMode_Deferred, nullptr, m_states.PointClamp());
	//m_uiManager.Render(spriteBatch);
	//spriteBatch->End();
} 