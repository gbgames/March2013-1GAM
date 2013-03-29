#ifndef GB_VIEWCONFIGURATION_H
#define GB_VIEWCONFIGURATION_H

#include "IViewConfiguration.h"

#include <map>

namespace GBLib
{

class IMenuModel;
class ButtonControl;

class ViewConfiguration : public IViewConfiguration
{
	public:
		ViewConfiguration(IMenuModel & menuModel);
		virtual ~ViewConfiguration();

	public:
		virtual void registerButtonControl(Option optionID, std::string disabledImage, std::string availableImage, std::string hotImage, std::string activeImage, Rectangle dimensions, std::string hotKey = ""); 
		virtual void registerText(std::string text, Point renderPos, std::string fontName, int red, int green, int blue, Point hotSpot);
		virtual int numWidgets() const;
		virtual int numTextWidgets() const;
		virtual ButtonControlConfigurations getButtonControlConfigurations();
		virtual TextLabelConfigurations getTextWidgets();
	
	public:
		ButtonControlConfigurations m_buttonControlConfigurations;
		TextLabelConfigurations m_textLabels;
};

}

#endif
