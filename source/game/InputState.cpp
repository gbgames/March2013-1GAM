#include "InputState.h"

InputState::InputState(IKeyboardState * keyboardState) :
	m_mousePosition(0, 0),
	m_mouseButtonStatus(0),
	m_hotID(BID_NONE),
	m_activeID(BID_NONE),
	m_keyboardState(keyboardState)
{
}

InputState::InputState(int x, int y, int button) :
	m_mousePosition(x, y),
	m_mouseButtonStatus(button),
	m_hotID(BID_NONE),
	m_activeID(BID_NONE),
	m_keyboardState(0)
{
}

InputState::~InputState()
{
}

Point InputState::position()
{
	return m_mousePosition;
}

int InputState::mouseButtonStatus()
{
	return m_mouseButtonStatus;
}

void InputState::setMousePosition(int x, int y)
{
	m_mousePosition.setPosition(x, y);
}

void InputState::setMouseButton(int state)
{
	m_mouseButtonStatus = state;
}

void InputState::setHot(int controlID)
{
	m_hotID = controlID;
}

int InputState::getHotID()
{
	return m_hotID;
}

void InputState::setActive(int controlID)
{
	m_activeID = controlID;
}

int InputState::getActiveID()
{
	return m_activeID;
}

IKeyboardState * InputState::keyboardState()
{
	return m_keyboardState;
}
