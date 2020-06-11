#include "pch.h"
#include "UIManager.h"

#include "IUIObject.h"

using namespace DirectX;

UIManager::UIManager() :
	m_currentID(-1), // First ID will be 0
	m_clearAtEnd(false)
{
}

UIManager::~UIManager()
{
	Clear();
}

void UIManager::Update(float deltaTime,
					   std::unique_ptr<InputState> & inputState)
{
	for (auto object : m_UIObjects)
	{
		// If objects exists and Update() returns true (so object is ready to be removed)
		if (object.second && object.second->Update(deltaTime, inputState))
			Remove(object.first);
	}

	if (m_clearAtEnd)
	{
		Clear();
		m_clearAtEnd = false;
	}
}

void UIManager::Render(SpriteBatch* spriteBatch)
{
	for (const auto& object : m_UIObjects)
		object.second->Draw(spriteBatch);
}

void UIManager::Resize(float width, float height, float oldWidth, float oldHeight)
{
	for (auto object : m_UIObjects)
	{
		object.second->Resize(width, height, oldWidth, oldHeight);
	}
}

int UIManager::Add(std::shared_ptr<IUIObject> newUI)
{
	// Increment current ID number; no UI object will ever have the same ID, and it isn't feasible that int max would ever be reached
	m_currentID++;

	// Create std::pair that houses UI object and ID
	std::pair<int, std::shared_ptr<IUIObject>> ui;
	ui.first = m_currentID;
	ui.second = newUI;

	m_UIObjects.push_back(ui);
	
	// Return ID so that UI object can be interacted with later.
	return m_currentID;
}

IUIObject* UIManager::Get(int id) {
	for (auto object : m_UIObjects) {
		if (object.first == id) {
			return object.second.get();
		}
	}

	return nullptr;
}

void UIManager::Remove(int id)
{
	int currentID;
	for (auto object : m_UIObjects)
	{
		currentID = object.first;
		if (currentID == id)
		{
			//auto itr = std::find(m_UIObjects.begin(), m_UIObjects.end(), object);
			m_UIObjects.erase(std::find(m_UIObjects.begin(), m_UIObjects.end(), object)); // Get iterator of object and erase from vector
		}
	}
}

void UIManager::Clear()
{
	m_UIObjects.clear();
	m_UIObjects.shrink_to_fit();
}