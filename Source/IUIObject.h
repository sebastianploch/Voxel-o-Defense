#pragma once

// Interface for all UI
class IUIObject
{
public:
	IUIObject();
	~IUIObject();

	virtual bool Update(float deltaTime, std::unique_ptr<InputState> & inputState) = 0;
	virtual void Draw(DirectX::SpriteBatch* spriteBatch) = 0;

	void SetVisible(bool visible) { m_isVisible = visible; }
protected:
	DirectX::SimpleMath::Vector2	m_screenPos; // 2D position on screen
	float							m_lifeTime; // Keeps track of object lifetime; -1 if not used
	float							m_lifeTimeTimer; // Counts down from object lifetime; if m_lifeTime is set, the object is removed once this reaches 0
	bool							m_isVisible; // If true, render
};