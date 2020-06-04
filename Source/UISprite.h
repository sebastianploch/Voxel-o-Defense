#pragma once

#include "IUIObject.h"

class UISprite : public IUIObject
{
public:
	UISprite();
	~UISprite();

	bool Update(float deltaTime, std::unique_ptr<InputState>& inputState) override;
	void Draw(DirectX::SpriteBatch* spriteBatch) override;

	// Move to screenPos and initialise sprite with texture
	void Initialise(DirectX::SimpleMath::Vector2 screenPos,
					const wchar_t* texturePath,
					ID3D11Device* device,
					float lifeTime = -1.0f,
					int width = -1,
					int height = -1);

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	DirectX::SimpleMath::Vector2 GetOrigin() { return m_origin; }

	void SetScale(float newScale) { m_scale = newScale; }
	void SetRotation(float newRotation) { m_rotation = newRotation; }
	void SetTint(DirectX::SimpleMath::Color newTint) { m_tint = newTint; }

	// Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture() { return m_texture; }
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;
	int													m_width;
	int													m_height;

	DirectX::SimpleMath::Vector2						m_origin;
	float												m_rotation;
	float												m_scale;
	DirectX::SimpleMath::Color							m_tint;
};