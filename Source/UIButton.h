#pragma once

#include "IUIObject.h"
#include "Subject.h"

class UISprite;
class UIText;

enum class BUTTONSTATE
{
	INACTIVE = 0,
	HOVER,
	CLICKED
};

class UIButton :
	public IUIObject,
	public Subject
{
public:
	UIButton();
	~UIButton();

	bool Update(float deltaTime, std::unique_ptr<InputState>& inputState) override;
	void Draw(DirectX::SpriteBatch* spriteBatch) override;

	void Initialise(DirectX::SimpleMath::Vector2 screenPos,
					const wchar_t* texturePath,
					const wchar_t* fontPath,
					const wchar_t* text,
					ID3D11Device* device,
					float lifeTime = -1.0f);
private:
	std::shared_ptr<UISprite>		m_sprite;
	std::shared_ptr<UIText>			m_text;

	DirectX::SimpleMath::Rectangle	m_bounds; // Defines clickable area

	BUTTONSTATE						m_state;
};