#include "Point.h"

#include <cmath>

Point::Point():
	m_xPos(0.0f),
	m_yPos(0.0f),
	m_zPos(0.0f)
{
}

Point::Point(float x, float y, float z):
	m_xPos(x),
	m_yPos(y),
	m_zPos(z)
{
}

Point::~Point()
{
}

float Point::X() const
{
	return m_xPos;
}

float Point::Y() const
{
	return m_yPos;
}

float Point::Z() const
{
	return m_zPos;
}

int Point::intX() const
{
	return roundf(m_xPos);
}

int Point::intY() const
{
	return roundf(m_yPos);
}

int Point::intZ() const
{
	return roundf(m_zPos);
}

int Point::floorX() const
{
	return floor(m_xPos);
}

int Point::floorY() const
{
	return floor(m_yPos);
}

int Point::floorZ() const
{
	return floor(m_zPos);
}

void Point::setX(float value)
{
	m_xPos = value;
}

void Point::setY(float value)
{
	m_yPos = value;
}

void Point::setZ(float value)
{
	m_zPos = value;
}

void Point::setPosition(float x, float y, float z)
{
	setX(x);
	setY(y);
	setZ(z);
}

void Point::moveBy(float x, float y, float z)
{
	m_xPos += x;
	m_yPos += y;
	m_zPos += z;
}

void Point::moveBy(Point offset)
{
	moveBy(offset.X(), offset.Y(), offset.Z());
}

bool Point::operator==(const Point& other) const
{
	float precision = 0.0001f;

	return ((std::fabs(this->m_xPos - other.m_xPos) < precision) &&
			(std::fabs(this->m_yPos - other.m_yPos) < precision) &&
			(std::fabs(this->m_zPos - other.m_zPos) < precision)); 
}

bool Point::operator!=(const Point& other) const
{
	return !(*this == other);
}

bool Point::operator<(const Point& other) const
{
	float precision = 0.0001f;
	bool isLessThan(this->Z() < other.Z());
	if (std::fabs(this->Z() - other.Z()) < precision)
	{
		isLessThan = this->Y() < other.Y();
		if (std::fabs(this->Y() - other.Y()) < precision)
		{
			isLessThan = this->X() < other.X();
		}
	}
	return isLessThan;
}

double calculateDistance(Point start, Point target)
{
	return std::sqrt((start.X() - target.X())*(start.X() - target.X()) + (start.Y() - target.Y())*(start.Y() - target.Y())); 
}

double calculateDistanceSquared(Point start, Point target)
{
	return (start.X() - target.X())*(start.X() - target.X()) + (start.Y() - target.Y())*(start.Y() - target.Y()); 
}

double calculateDotProduct(Point a, Point b)
{
	return (a.X()*b.X() + a.Y()*b.Y());
}
