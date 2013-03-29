#include "KeyboardState.h"


KeyboardState::KeyboardState(int numKeys) : 
	m_keys(numKeys, 0)
{
}

KeyboardState::~KeyboardState()
{
}

void KeyboardState::setKeyDown(int index)
{
	m_keys[index] = 1;
}

void KeyboardState::setKeyDown(std::string keyName)
{
	setKeyDown(m_keyNameMapping[keyName]);
}

void KeyboardState::setKeyUp(int index)
{
	m_keys[index] = 0;
}

void KeyboardState::setKeyUp(std::string keyName)
{
	setKeyUp(m_keyNameMapping[keyName]);
}

bool KeyboardState::getKeyState(int index)
{
	return (m_keys.at(index) == 1);
}

bool KeyboardState::getKeyState(std::string keyName)
{
	return getKeyState(m_keyNameMapping[keyName]);
}

void KeyboardState::setKeyNameMapping(KeyNameMap mapping)
{
	m_keyNameMapping = mapping;
}
