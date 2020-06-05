#include "pch.h"
#include "UIText.h"

using namespace DirectX;

UIText::UIText() :
	IUIObject(),
	m_font(nullptr),
	m_text(L""),
	m_origin(SimpleMath::Vector2(0.0f, 0.0f)),
	m_scale(1.0f),
	m_isOutlined(false)
{
}

UIText::~UIText()
{
	m_font.reset();
}

void UIText::OnNotify(std::shared_ptr<Event> event)
{
	switch (event->m_type)
	{
	case EVENTTYPE::CLICK:

		break;
	case EVENTTYPE::VALCHANGE:
		std::shared_ptr<ValChangeEvent> valChangeEvent = std::static_pointer_cast<ValChangeEvent>(event);
		SetText(std::to_wstring(valChangeEvent->m_newVal).c_str());
		break;
	}
}

bool UIText::Update(float deltaTime,
					std::unique_ptr<InputState>& inputState)
{
	if (m_lifeTime > 0.0f)
	{
		m_lifeTimeTimer -= deltaTime;

		if (m_lifeTimeTimer <= 0.0f)
			return true;
	}

	return false;
}

void UIText::Draw(SpriteBatch*
	spriteBatch)
{
	if (m_isOutlined)
	{
		m_font->DrawString(spriteBatch,
						   m_text,
						   m_screenPos + SimpleMath::Vector2(m_scale, m_scale),
						   m_outlineColor,
						   0.0f,
						   m_origin,
						   m_scale);
		m_font->DrawString(spriteBatch,
						   m_text,
						   m_screenPos + SimpleMath::Vector2(-m_scale, m_scale),
						   m_outlineColor,
						   0.0f,
						   m_origin,
						   m_scale);
		m_font->DrawString(spriteBatch,
						   m_text,
						   m_screenPos + SimpleMath::Vector2(-m_scale, -m_scale),
						   m_outlineColor,
						   0.0f,
						   m_origin,
						   m_scale);
		m_font->DrawString(spriteBatch,
						   m_text,
						   m_screenPos + SimpleMath::Vector2(m_scale, -m_scale),
						   m_outlineColor,
						   0.0f,
						   m_origin,
						   m_scale);
	}

	m_font->DrawString(spriteBatch,
					   m_text,
					   m_screenPos,
					   m_color,
					   0.0f,
					   m_origin,
					   m_scale);
}

void UIText::Initialise(DirectX::SimpleMath::Vector2 screenPos,
						const wchar_t* text,
						SimpleMath::Color color,
						const wchar_t* fontPath,
						ID3D11Device* device,
						float lifeTime,
						bool outline,
						SimpleMath::Color outlineColor)
{
	m_screenPos = screenPos;
	m_text = text;
	m_color = color;
	m_font = std::make_unique<SpriteFont>(device, fontPath);
	m_lifeTime = lifeTime;
	m_lifeTimeTimer = lifeTime;
	m_isOutlined = outline;
	m_outlineColor = outlineColor;

	m_origin = m_font->MeasureString(m_text) / 2.0f;
}