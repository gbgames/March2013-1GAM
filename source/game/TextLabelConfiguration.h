#ifndef GB_TEXTLABELCONFIGURATIONS_H
#define GB_TEXTLABELCONFIGURATIONS_H

#include "Color.h"
#include "Point.h"

#include <string>
#include <vector>

struct TextLabelConfiguration
{
	TextLabelConfiguration(std::string text_, Point position_, std::string fontID_, Color color_, Point hotSpot_) :
		text(text_),
		renderPosition(position_),
		fontID(fontID_),
		textColor(color_),
		hotSpot(hotSpot_)
	{
	}

	std::string text;
	Point renderPosition;
	std::string fontID;
	Color textColor;
	Point hotSpot;
};

typedef std::vector<TextLabelConfiguration> TextLabelConfigurations;

#endif 
