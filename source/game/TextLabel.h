#ifndef GB_TEXTLABEL_H
#define GB_TEXTLABEL_H

#include "Point.h"

#include <string>

class IRenderCommand;

class TextLabel
{
	public:
		TextLabel(Point position, std::string fontID, int red, int green, int blue, Point hotSpot);
		~TextLabel();
	
	public:
		std::string font() const;
		Point position() const;
		int red() const;
		int green() const;
		int blue() const;
		Point hotSpot() const;

		IRenderCommand * draw(std::string text);

	public:
		Point m_position;
		std::string m_font;
		int m_red;
		int m_green;
		int m_blue;
		Point m_hotSpot;
};

#endif
