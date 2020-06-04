#include "pch.h"
#include "UISystem.h"

#include "IUIObject.h"

using namespace DirectX;

UISystem::UISystem() :
	m_currentID(0)
{

}

UISystem::~UISystem()
{
	m_UIObjects.clear();
	m_UIObjects.shrink_to_fit();
}

void UISystem::Update(float deltaTime, std::unique_ptr<InputState> & inputState)
{
	for (auto object : m_UIObjects)
	{
		// If objects exists and Update() returns true (so object is ready to be removed)
		if (object && object->Update(deltaTime, inputState))
			Remove(object);
	}
}

void UISystem::Render(SpriteBatch* spriteBatch)
{
	for (const auto& object : m_UIObjects)
		object->Draw(spriteBatch);
}

int UISystem::Add(std::shared_ptr<IUIObject> newUI)
{
	m_currentID++;
	m_UIObjects.push_back(newUI);
	
	return m_currentID;
}

void UISystem::Remove(std::shared_ptr<IUIObject> removeUI)
{
	m_UIObjects.erase(std::remove(m_UIObjects.begin(), m_UIObjects.end(), removeUI), m_UIObjects.end());
}

void UISystem::Remove(int id)
{
	for (auto object : m_UIObjects)
	{
		if (object->GetID() == id)
		{
			auto itr = std::find(m_UIObjects.begin(), m_UIObjects.end(), object);
			m_UIObjects.erase(itr);
		}
	}
}