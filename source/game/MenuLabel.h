#ifndef GB_MENULABEL_H
#define GB_MENULABEL_H

#include "TextLabel.h"

#include <string>

class MenuLabel 
{
	public:
		MenuLabel(std::string textCaption, Point position, std::string fontName, int red, int green, int blue, Point alignment);
		virtual ~MenuLabel();

	public:	
		virtual IRenderCommand * draw(Point offset = Point(0, 0));
		
		virtual Point position() const;
		virtual bool isVisible() const;
		virtual void setVisible(bool isVisible = true);

		std::string textCaption() const;
		std::string fontName() const;
		int red() const;
		int green() const;
		int blue() const;
		Point alignment() const;

	public:
		std::string m_caption;
		bool m_isVisible;
		Point m_position;
		TextLabel m_textLabel;
};

#endif
