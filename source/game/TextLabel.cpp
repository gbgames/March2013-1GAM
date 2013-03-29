#include "TextLabel.h"

#include "DrawTextCommand.h"

TextLabel::TextLabel(Point position, std::string fontID, int red, int green, int blue, Point hotSpot) :
	m_position(position),
	m_font(fontID),
	m_red(red),
	m_green(green),
	m_blue(blue),
	m_hotSpot(hotSpot)
{
}

TextLabel::~TextLabel()
{
}

std::string TextLabel::font() const
{
	return m_font;
}

Point TextLabel::position() const
{
	return m_position;
}

int TextLabel::red() const
{
	return m_red;
} 

int TextLabel::green() const
{
	return m_green;
} 

int TextLabel::blue() const
{
	return m_blue;
} 

Point TextLabel::hotSpot() const
{
	return m_hotSpot;
} 

IRenderCommand * TextLabel::draw(std::string text)
{
	DrawTextCommand * cmd = new DrawTextCommand(position(), hotSpot(), font(), text, red(), green(), blue());
	
	return cmd;
} 


