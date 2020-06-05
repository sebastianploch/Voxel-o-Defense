#pragma once

class IUIObject;

class UIManager
{
public:
	UIManager();
	~UIManager();

	void Update(float deltaTime, std::unique_ptr<InputState> & inputState);
	void Render(DirectX::SpriteBatch* spriteBatch);

	int Add(std::shared_ptr<IUIObject> newUI); // Adds the given UI element, then assigns and returns an ID to allow later interaction
	void Remove(int id); // Finds UI element using given ID and deletes it
	void Clear(); // Deletes all UI elements

private:
	std::vector<std::pair<int, std::shared_ptr<IUIObject>>> m_UIObjects; // Each pair holds an int ID and a pointer to a UI element
	int m_currentID; // Increments whenever a new UI element is added
};