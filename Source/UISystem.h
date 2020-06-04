#pragma once

class IUIObject;

class UISystem
{
public:
	UISystem();
	~UISystem();

	void Update(float deltaTime, std::unique_ptr<InputState> & inputState);
	void Render(DirectX::SpriteBatch* spriteBatch);

	int Add(std::shared_ptr<IUIObject> newUI);

	void Remove(std::shared_ptr<IUIObject> removeUI);
	void Remove(int id);

private:
	std::vector<std::shared_ptr<IUIObject>> m_UIObjects;
	int m_currentID;
};