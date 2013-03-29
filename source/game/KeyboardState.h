#ifndef GB_KEYBOARDSTATE_H
#define GB_KEYBOARDSTATE_H

#include "IKeyboardState.h"

class KeyboardState : public IKeyboardState
{
	public:
		KeyboardState(int numKeys);
		virtual ~KeyboardState(); 
	
	public:
		virtual void setKeyDown(int index);
		virtual void setKeyDown(std::string keyName);
		virtual void setKeyUp(int index);
		virtual void setKeyUp(std::string keyName);
		virtual bool getKeyState(int index);
		virtual bool getKeyState(std::string keyName);
		virtual void setKeyNameMapping(KeyNameMap mapping);

	public:
		Keys m_keys;
		KeyNameMap m_keyNameMapping;
};

#endif
