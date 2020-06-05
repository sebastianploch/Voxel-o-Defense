#include "pch.h"
#include "UISprite.h"

using namespace DirectX;

UISprite::UISprite() :
	IUIObject(),
	m_texture(nullptr),
	m_width(0),
	m_height(0),
	m_rotation(0.0f),
	m_scale(1.0f),
	m_tint(Colors::White),
	m_isStretched(false)
{
}

UISprite::~UISprite()
{
}

bool UISprite::Update(float deltaTime,
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

void UISprite::Draw(SpriteBatch*
	spriteBatch)
{
	if (m_isStretched)
	{
		RECT destRect;
		destRect.left = m_screenPos.x;
		destRect.top = m_screenPos.y;
		destRect.right = destRect.left + m_width;
		destRect.bottom = destRect.top + m_height;

		spriteBatch->Draw(m_texture.Get(),
			destRect,
			nullptr,
			m_tint,
			m_rotation,
			m_origin);
	}
	else
	{
		spriteBatch->Draw(m_texture.Get(),
			m_screenPos,
			nullptr,
			m_tint,
			m_rotation,
			m_origin,
			m_scale);
	}
}

void UISprite::Initialise(DirectX::SimpleMath::Vector2 screenPos,
						  const wchar_t* texturePath,
						  ID3D11Device* device,
						  float lifeTime,
						  int width,
						  int height)
{
	m_screenPos = screenPos;

	if (width < 0 || height < 0)
	{
		// To access texture info
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;

		DX::ThrowIfFailed(CreateDDSTextureFromFile(device,
			texturePath,
			resource.GetAddressOf(),
			m_texture.ReleaseAndGetAddressOf()));

		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
		DX::ThrowIfFailed(resource.As(&tex2D));

		CD3D11_TEXTURE2D_DESC texDesc;
		tex2D->GetDesc(&texDesc);

		m_width = texDesc.Width;
		m_height = texDesc.Height;
	}
	else
	{
		m_isStretched = true;

		DX::ThrowIfFailed(CreateDDSTextureFromFile(device,
			texturePath,
			nullptr,
			m_texture.ReleaseAndGetAddressOf()));

		m_width = width;
		m_height = height;
	}

	m_origin.x = (float)(m_width / 2);
	m_origin.y = (float)(m_height / 2);

	m_lifeTime = lifeTime;
	m_lifeTimeTimer = lifeTime;
}