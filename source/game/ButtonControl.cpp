#include "ButtonControl.h"

#include "DrawSpriteCommand.h"
#include "InputState.h"

using namespace GBLib;

ButtonControl::ButtonControl(Option option, std::string disabledBackground, std::string availableBackground, std::string hotBackground, std::string activeBackground, int x, int y, int w, int h) :
	m_option(option),
	m_dimensions(x, y, w, h),
	m_buttonState(CS_DEFAULT)
{
	m_backgrounds[CS_DEFAULT] = availableBackground;
	m_backgrounds[CS_HOT] = hotBackground;
	m_backgrounds[CS_ACTIVE] = availableBackground;
	m_backgrounds[CS_HOT_AND_ACTIVE] = activeBackground;
	m_backgrounds[CS_DISABLED] = disabledBackground;
}

ButtonControl::~ButtonControl()
{
}

bool ButtonControl::isActive() const
{
	return true;
}

bool ButtonControl::isVisible() const
{
	return true;
}

Point ButtonControl::position() const
{
	return m_dimensions.position();
}

Rectangle ButtonControl::dimensions() const
{
	return m_dimensions;
}

Option ButtonControl::ID() const
{
	return m_option;
}

bool ButtonControl::update(InputState & inputState)
{
	bool hasClicked(false);
	m_buttonState = CS_DEFAULT;
	if (m_dimensions.contains(inputState.position()))
	{
		m_buttonState = CS_HOT;
		inputState.setHot(m_option.ID());
		if (BID_NONE == inputState.getActiveID() && BUTTON_DOWN == inputState.mouseButtonStatus())
		{
			m_buttonState = CS_HOT_AND_ACTIVE;
			inputState.setActive(m_option.ID());
		}
	}

	if (m_option.ID() == inputState.getHotID() && 
		m_option.ID() == inputState.getActiveID() && 
		BUTTON_UP == inputState.mouseButtonStatus())
	{
		hasClicked = true;
	}

	return hasClicked;
}

IRenderCommand * ButtonControl::draw()
{
	DrawSpriteCommand * cmd = new DrawSpriteCommand();
	cmd->setDrawComplete(m_backgrounds[m_buttonState], m_dimensions.position());
	return cmd;
}
