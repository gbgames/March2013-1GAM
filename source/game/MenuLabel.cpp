#include "MenuLabel.h"

MenuLabel::MenuLabel(std::string textCaption, Point position, std::string fontName, int red, int green, int blue, Point alignment) :
	m_caption(textCaption),
	m_isVisible(true),
	m_position(position),
	m_textLabel(position, fontName, red, green, blue, alignment)
{
}

MenuLabel::~MenuLabel()
{
}

IRenderCommand * MenuLabel::draw(Point offset)
{
	return m_textLabel.draw(m_caption);
}

Point MenuLabel::position() const
{
	return m_textLabel.position();
}

bool MenuLabel::isVisible() const
{
	return m_isVisible;
}

void MenuLabel::setVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

std::string MenuLabel::textCaption() const
{
	return m_caption;
}

std::string MenuLabel::fontName() const
{
	return m_textLabel.font();
}

int MenuLabel::red() const
{
	return m_textLabel.red();
}

int MenuLabel::green() const
{
	return m_textLabel.green();
}

int MenuLabel::blue() const
{
	return m_textLabel.blue();
}

Point MenuLabel::alignment() const
{
	return m_textLabel.hotSpot();
}


