#include "UnitTest++.h"

#include "ViewConfiguration.h"

#include "CommandIDs.h"
#include "MenuModel.h"
#include "Option.h"

using namespace GBLib;

TEST(ViewConfigurationReturnsNoWidgets)
{
	Options options;
	Command::CommandIDs commands;
	MenuModel menuModel(options, commands);
	ViewConfiguration viewConfiguration(menuModel);

	CHECK_EQUAL(0, viewConfiguration.numWidgets());
	CHECK_EQUAL(0, viewConfiguration.numTextWidgets());
}

TEST(ViewConfigurationProvidesButtonWidgetInformation)
{
	Option OPTION_1(1);
	Options options;
	options.push_back(Option(OPTION_1));

	Command::CommandIDs commands;
	commands.push_back(Command::QUIT_APPLICATION);

	MenuModel menuModel(options, commands);
	ViewConfiguration viewConfiguration(menuModel);

	std::string disabledImage1("DISABLED_1");
	std::string activeImage1("ACTIVE_1");
	std::string hotImage1("HOT_1");
	std::string availableImage1("AVAILABLE_1");
	Rectangle dimensions1(10, 10, 20, 20);
	std::string hotKey1("G");

	viewConfiguration.registerButtonControl(OPTION_1, disabledImage1, availableImage1, hotImage1, activeImage1, dimensions1, hotKey1);

	CHECK_EQUAL(1, viewConfiguration.numWidgets());

	ButtonControlConfigurations buttonControlConfigurations = viewConfiguration.getButtonControlConfigurations();

	CHECK_EQUAL(1, buttonControlConfigurations.size());

	ButtonControlConfiguration buttonConfiguration = buttonControlConfigurations.at(0);

	CHECK_EQUAL(disabledImage1.c_str(), buttonConfiguration.disabledSpriteID.c_str());
	CHECK_EQUAL(availableImage1.c_str(), buttonConfiguration.availableSpriteID.c_str());
	CHECK_EQUAL(hotImage1.c_str(), buttonConfiguration.hotSpriteID.c_str());
	CHECK_EQUAL(activeImage1.c_str(), buttonConfiguration.activeSpriteID.c_str());
	CHECK_EQUAL(hotKey1.c_str(), buttonConfiguration.hotKey.c_str());
	CHECK(dimensions1 == buttonConfiguration.buttonDimensionsAndPosition);
}

TEST(ViewConfigurationProvidesTextWidgetInformation)
{
	Options options;
	Command::CommandIDs commands;
	MenuModel menuModel(options, commands);
	ViewConfiguration viewConfiguration(menuModel);

	std::string fooText("FooText");
	Point fooTextPosition(10, 20, 30);
	std::string fontName("FooFont");
	int redness(11), greenness(12), blueness(13);
	Point hotSpot(1.0, 0.5);


	viewConfiguration.registerText(fooText, fooTextPosition, fontName, redness, greenness, blueness, hotSpot);

	CHECK_EQUAL(1, viewConfiguration.numTextWidgets());

	TextLabelConfigurations textWidgets = viewConfiguration.getTextWidgets();

	CHECK_EQUAL(1, textWidgets.size());

	TextLabelConfiguration textWidget = textWidgets.at(0);

	CHECK_EQUAL(fooText.c_str(), textWidget.text.c_str());
	CHECK(fooTextPosition == textWidget.renderPosition);
	CHECK_EQUAL(fontName, textWidget.fontID);
	CHECK_EQUAL(redness, textWidget.textColor.redness);
	CHECK_EQUAL(greenness, textWidget.textColor.greenness);
	CHECK_EQUAL(blueness, textWidget.textColor.blueness);
	CHECK_EQUAL(redness, textWidget.textColor.redness);
	CHECK(hotSpot == textWidget.hotSpot);
}
