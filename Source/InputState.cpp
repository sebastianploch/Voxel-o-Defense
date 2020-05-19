#include "pch.h"
#include "InputState.h"

using namespace DirectX;


InputState::InputState(const HWND& window) :
	m_mouse(std::make_unique<Mouse>()),
	m_keyboard(std::make_unique<Keyboard>())
{
	m_mouse->SetWindow(window);
}

InputState::~InputState()
{
}

void InputState::Update()
{
	m_mouseTracker.Update(m_mouse->GetState());
	m_keyboardTracker.Update(m_keyboard->GetState());
}

void InputState::Reset()
{
	m_mouseTracker.Reset();
	m_keyboardTracker.Reset();
}