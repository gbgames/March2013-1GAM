#ifndef GB_IVIEWCONFIGURATION_H
#define GB_IVIEWCONFIGURATION_H

#include "ButtonControlConfiguration.h"
#include "Option.h"
#include "Rectangle.h"
#include "TextLabelConfiguration.h"
#include <string>

namespace GBLib
{

class IViewConfiguration
{
	public:
		virtual ~IViewConfiguration() {}
	
	public:
		virtual void registerButtonControl(Option optionID, std::string disabledImage, std::string availableImage, std::string hotImage, std::string activeImage, Rectangle dimensions, std::string hotKey = "") = 0;
		virtual void registerText(std::string text, Point renderPos, std::string fontName, int red, int green, int blue, Point hotSpot) = 0;

		virtual int numWidgets() const = 0;
		virtual int numTextWidgets() const = 0;
		virtual ButtonControlConfigurations getButtonControlConfigurations() = 0;
		virtual TextLabelConfigurations getTextWidgets() = 0;
};

}

#endif
