#include "pch.h"
#include "Input.h"

namespace nu {
	bool Input::Initialize() {
		int numKeys;
		const bool* keyState = SDL_GetKeyboardState(&numKeys);
		m_keyStates.resize(numKeys);

		std::copy(keyState, keyState + numKeys, m_keyStates.begin());
		m_prevKeyStates = m_keyStates;

		return true;
	}

	void Input::Shutdown() {
		// Handled in Renderer::Shutdown()
	}

	void Input::Update() {
		m_prevKeyStates = m_keyStates; // store last frame key states
		const bool* keyState = SDL_GetKeyboardState(NULL);
		std::copy(keyState, keyState + m_keyStates.size(), m_keyStates.begin());

		m_prevButtonStates = m_buttonStates; // store last frame button states
		m_buttonStates = SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);

	}
	uint32_t Input::GetButtonBit(MouseButton button) const
	{

		// 1 -> 0001
		// 2 -> 0010
		// 3 -> 0100
		return SDL_BUTTON_MASK((uint32_t)button);
	}
}