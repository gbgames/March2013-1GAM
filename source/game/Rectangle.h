#ifndef GB_RECTANGLE_H
#define GB_RECTANGLE_H

#include "Point.h"

class Rectangle
{
	public:
		Rectangle();
		Rectangle(float x, float y, float w, float h);
		Rectangle(Point corner1, Point corner2);
		~Rectangle();

		Point position() const;
		float width() const;
		float height() const;

		void setRectangle(float x, float y, float w, float h);
		void setRectangle(Point corner1, Point corner2);
		void setWidth(float value);
		void setHeight(float value);

		void moveBy(float x, float y);
		void moveBy(Point offset);

		bool contains(float x, float y);
		bool contains(Point point);

		bool operator==(const Rectangle& other) const;
		bool operator!=(const Rectangle& other) const;

	public:

		Point m_position;
		float m_width, m_height;
};

#endif
