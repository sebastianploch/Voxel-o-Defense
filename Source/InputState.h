#pragma once


class InputState
{
public:
	InputState(const HWND& window);
	~InputState();

	void Update();
	void Reset();

	inline DirectX::Mouse& GetMouse() const { return *m_mouse; }
	inline const DirectX::Mouse::ButtonStateTracker& GetMouseState() const { return m_mouseTracker; }

	inline DirectX::Keyboard& GetKeyboard() const { return *m_keyboard; }
	inline const DirectX::Keyboard::KeyboardStateTracker& GetKeyboardState() const { return m_keyboardTracker; }

private:
	std::unique_ptr<DirectX::Mouse>			m_mouse;
	std::unique_ptr<DirectX::Keyboard>		m_keyboard;

	DirectX::Mouse::ButtonStateTracker		m_mouseTracker;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
};