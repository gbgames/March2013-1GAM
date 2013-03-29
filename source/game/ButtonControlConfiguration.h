#ifndef GB_BUTTONCONTROLCONFIGURATION_H
#define GB_BUTTONCONTROLCONFIGURATION_H

#include "Option.h"
#include "Rectangle.h"

#include <string>
#include <vector>

namespace GBLib
{

struct ButtonControlConfiguration
{
	Option option;
	std::string disabledSpriteID;
	std::string availableSpriteID;
	std::string hotSpriteID;
	std::string activeSpriteID;
	Rectangle buttonDimensionsAndPosition;
	std::string hotKey;

	ButtonControlConfiguration(Option option_, 
		std::string disabled_, 
		std::string available_, 
		std::string hot_,
		std::string active_,
		Rectangle positionAndDimension_,
		std::string hotKey_) :
	option(option_),
	disabledSpriteID(disabled_),
	availableSpriteID(available_),
	hotSpriteID(hot_),
	activeSpriteID(active_),
	buttonDimensionsAndPosition(positionAndDimension_),
	hotKey(hotKey_)
	{
	}
};

typedef std::vector<ButtonControlConfiguration> ButtonControlConfigurations;

}

#endif
