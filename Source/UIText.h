#pragma once

#include "IUIObject.h"

class UIText : public IUIObject
{
public:
	UIText();
	~UIText();

	bool Update(float deltaTime, std::unique_ptr<InputState>& inputState) override;
	void Draw(DirectX::SpriteBatch* spriteBatch) override;

	void Initialise(DirectX::SimpleMath::Vector2 screenPos,
					const wchar_t* text,
					DirectX::SimpleMath::Color color,
					const wchar_t* fontPath,
					ID3D11Device* device,
					float lifeTime = -1.0f,
					bool outline = false,
					DirectX::SimpleMath::Color outlineColor = DirectX::SimpleMath::Color(DirectX::Colors::Black));
	
	void SetColor(DirectX::SimpleMath::Color newColor) { m_color = newColor; }
	void SetScale(float newScale) { m_scale = newScale; }
	void SetOutline(bool isOutlined,
					 DirectX::SimpleMath::Color outlineColor = DirectX::SimpleMath::Color(DirectX::Colors::Black)) { m_isOutlined = isOutlined; m_outlineColor = outlineColor; }

private:
	std::unique_ptr<DirectX::SpriteFont>	m_font;
	const wchar_t*							m_text;
	DirectX::SimpleMath::Color				m_color;
	DirectX::SimpleMath::Color				m_outlineColor;
	DirectX::SimpleMath::Vector2			m_origin;
	float									m_scale;

	bool									m_isOutlined;
};