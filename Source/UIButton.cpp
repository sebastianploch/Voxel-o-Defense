#include "pch.h"
#include "UIButton.h"

#include "UISprite.h"
#include "UIText.h"

using namespace DirectX;

UIButton::UIButton() :
	IUIObject(),
	m_bounds(SimpleMath::Rectangle()),
	m_sprite(std::make_shared<UISprite>()),
	m_text(std::make_shared<UIText>()),
	m_clickedSubject(new Subject()),
	m_state(BUTTONSTATE::INACTIVE)
{
}

UIButton::~UIButton()
{
}

bool UIButton::Update(float deltaTime,
			   std::unique_ptr<InputState>& inputState)
{
	Mouse::State mouseState = inputState->GetMouse().GetState();

	// If has a lifetime, decrement timer
	if (m_lifeTime > 0.0f)
	{
		m_lifeTimeTimer -= deltaTime;

		if (m_lifeTimeTimer <= 0.0f)
			return true;
	}

	switch (m_state)
	{
	case BUTTONSTATE::INACTIVE:
		// Check for hover over
		if (m_bounds.Contains(SimpleMath::Vector2((float)mouseState.x, (float)mouseState.y)))
		{
			m_state = BUTTONSTATE::HOVER;
			m_sprite->SetTint(SimpleMath::Color(Colors::LightGray));
		}

		break;
	case BUTTONSTATE::HOVER:
		// Check if no longer hovering over
		if (!m_bounds.Contains(SimpleMath::Vector2((float)mouseState.x, (float)mouseState.y)))
		{
			m_state = BUTTONSTATE::INACTIVE;
			m_sprite->SetTint(SimpleMath::Color(Colors::White));

			break; // Skip click check
		}

		// Check for click
		if (mouseState.leftButton)
		{
			m_state = BUTTONSTATE::CLICKED;
			m_sprite->SetTint(SimpleMath::Color(Colors::DarkGray));
		}

		break;
	case BUTTONSTATE::CLICKED:
		// Check for mouse up
		if (!mouseState.leftButton)
		{
			m_clickedSubject->Notify(new ClickEvent());
			m_state = BUTTONSTATE::INACTIVE;
			m_sprite->SetTint(SimpleMath::Color(Colors::White));

			// Check if no longer hovering over
			if (!m_bounds.Contains(SimpleMath::Vector2((float)mouseState.x, (float)mouseState.y)))
			{
				m_state = BUTTONSTATE::INACTIVE;
				m_sprite->SetTint(SimpleMath::Color(Colors::White));

				break; // Skip click check
			}
			else
			{
				m_state = BUTTONSTATE::HOVER;
				m_sprite->SetTint(SimpleMath::Color(Colors::LightGray));
			}
		}

		break;
	}

	return false;
}

void UIButton::Draw(DirectX::SpriteBatch* spriteBatch)
{
	if (!m_isVisible)
		return;

	m_sprite->Draw(spriteBatch);
	m_text->Draw(spriteBatch);
}

void UIButton::Resize(float width, float height, float oldWidth, float oldHeight)
{
	float widthMultiplier = GetRatio(width, oldWidth);
	float heightMultiplier = GetRatio(height, oldHeight);

	m_screenPos.x *= widthMultiplier;
	m_screenPos.y *= heightMultiplier;

	m_sprite->Resize(width, height, oldWidth, oldHeight);
	m_text->Resize(width, height, oldWidth, oldHeight);

	SimpleMath::Vector2 origin = m_sprite->GetOrigin();
	m_screenPos;
	m_bounds = SimpleMath::Rectangle((long)(m_screenPos.x - origin.x), (long)(m_screenPos.y - origin.y), (long)m_sprite->GetWidth(), (long)m_sprite->GetHeight());
	m_bounds;
}

void UIButton::Initialise(DirectX::SimpleMath::Vector2 screenPos,
						  const wchar_t* texturePath,
						  const wchar_t* fontPath,
						  const wchar_t* text,
						  ID3D11Device* device,
						  float lifeTime)
{
	m_screenPos = screenPos;

	m_sprite->Initialise(screenPos, texturePath, device);
	m_text->Initialise(screenPos, text, SimpleMath::Color(Colors::White), fontPath, device, true);

	m_lifeTime = lifeTime;
	m_lifeTimeTimer = lifeTime;

	SimpleMath::Vector2 origin = m_sprite->GetOrigin();

	m_bounds = SimpleMath::Rectangle((long)(screenPos.x - origin.x), (long)(screenPos.y - origin.y), (long)m_sprite->GetWidth(), (long)m_sprite->GetHeight());
}