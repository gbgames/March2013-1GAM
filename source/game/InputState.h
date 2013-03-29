#ifndef GB_INPUTSTATE_H
#define GB_INPUTSTATE_H

#include "Point.h"

class IKeyboardState;

enum ButtonStates
{
	BUTTON_UP = 0,
	BUTTON_DOWN
};

enum ButtonID
{
	BID_NONE = 999,
	BID_UNAVAILABLE
};

class InputState
{
	public:
		InputState(IKeyboardState * keyboardState = 0);
		InputState(int x, int y, int button); 

		~InputState();

	public:
		Point position();
		int mouseButtonStatus();

		void setMousePosition(int x, int y);
		void setMouseButton(int state);

		void setHot(int controlID);
		int getHotID();

		void setActive(int controlID);
		int getActiveID();

		IKeyboardState * keyboardState();

	public:

		Point m_mousePosition;
		int m_mouseButtonStatus;
		int m_hotID;
		int m_activeID;
		IKeyboardState * m_keyboardState;
};

#endif
