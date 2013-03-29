#include "ViewConfiguration.h"

using namespace GBLib;

ViewConfiguration::ViewConfiguration(IMenuModel & menuModel)
{
}

ViewConfiguration::~ViewConfiguration()
{
}


void ViewConfiguration::registerButtonControl(Option optionID, std::string disabledImage, std::string availableImage, std::string hotImage, std::string activeImage, Rectangle dimensions, std::string hotKey)
{
	m_buttonControlConfigurations.push_back(ButtonControlConfiguration(optionID, disabledImage, availableImage, hotImage, activeImage, dimensions, hotKey));
}

void ViewConfiguration::registerText(std::string text, Point renderPos, std::string fontName, int red, int green, int blue, Point hotSpot)
{
	m_textLabels.push_back(TextLabelConfiguration(text, renderPos, fontName, Color(red, green, blue), hotSpot));
}

int ViewConfiguration::numWidgets() const
{
	return m_buttonControlConfigurations.size();
}

int ViewConfiguration::numTextWidgets() const
{
	return m_textLabels.size();
}

ButtonControlConfigurations ViewConfiguration::getButtonControlConfigurations()
{
	return m_buttonControlConfigurations;
}

TextLabelConfigurations ViewConfiguration::getTextWidgets()
{
	return m_textLabels;
}
