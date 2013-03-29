#ifndef GB_BUTTONCONTROL_H
#define GB_BUTTONCONTROL_H

#include "CommandIDs.h"
#include "Option.h"
#include "Point.h"
#include "Rectangle.h"
#include <map>
#include <string>

class InputState;
class IRenderCommand;

namespace GBLib
{

enum ControlState
{
	CS_DEFAULT,
	CS_HOT,
	CS_ACTIVE,
	CS_HOT_AND_ACTIVE,
	CS_DISABLED
};

class ButtonControl
{
	public:
		ButtonControl(Option option, std::string disabledBackground, std::string availableBackground, std::string hotBackground, std::string activeBackground, int x, int y, int w, int h); 
		virtual  ~ButtonControl(); 
	
	public:
		virtual bool isActive() const; 
		virtual bool isVisible() const;  
		virtual Point position() const; 
		virtual Rectangle dimensions() const; 
		virtual Option ID() const;
		virtual bool update(InputState & inputState);
		virtual IRenderCommand * draw();

	private:
		Option m_option;
		Rectangle m_dimensions;
		int m_buttonState;
		std::map<int, std::string> m_backgrounds;

};

}

#endif
