#include "Rectangle.h"

#include <algorithm>

Rectangle::Rectangle() : m_position(0, 0), m_width(0), m_height(0)
{
}

Rectangle::Rectangle(float x, float y, float w, float h) : m_position(x, y), m_width(w), m_height(h)
{
}

Rectangle::Rectangle(Point corner1, Point corner2)
{
	setRectangle(corner1, corner2);
}

Rectangle::~Rectangle()
{
}

Point Rectangle::position() const 
{
	return m_position;
}

float Rectangle::width() const 
{
	return m_width;
}

float Rectangle::height() const 
{
	return m_height;
}

void Rectangle::setRectangle(float x, float y, float w, float h)
{
	setRectangle(Point(x, y), Point(x+w, y+h));
}

void Rectangle::setRectangle(Point corner1, Point corner2)
{
	float left = std::min(corner1.X(), corner2.X());
	float right = std::max(corner1.X(), corner2.X());
	float top = std::min(corner1.Y(), corner2.Y());
	float bottom = std::max(corner1.Y(), corner2.Y());

	m_position.setPosition(left, top);
	m_width = right - left;
	m_height = bottom - top;
}

void Rectangle::setWidth(float value)
{
	if (value >= 0)
	{
		m_width = value;
	}
}

void Rectangle::setHeight(float value)
{
	if (value >= 0)
	{
		m_height = value;
	}
}

void Rectangle::moveBy(float x, float y)
{
	m_position.moveBy(x, y);
}

void Rectangle::moveBy(Point offset)
{
	moveBy(offset.X(), offset.Y());
}

bool Rectangle::contains(float x, float y)
{
	return (x >= m_position.X() && x <= m_position.X() + width() &&
				y >= m_position.Y() && y <= m_position.Y() + height());
}

bool Rectangle::contains(Point point)
{
	return contains(point.X(), point.Y());
}

bool Rectangle::operator==(const Rectangle& other) const
{
	return (this->position() == other.position() &&
				this->width() == other.width() &&
				this->height() == other.height());
}

bool Rectangle::operator!=(const Rectangle& other) const
{
	return !(*this == other);
}
