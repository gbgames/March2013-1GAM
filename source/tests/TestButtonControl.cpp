#include "UnitTest++.h"
#include "ButtonControl.h"
#include "CommandIDs.h"
#include "DrawSpriteCommand.h"
#include "InputState.h"
#include "IRenderCommand.h"

using namespace GBLib;

class ButtonControlFixture
{
	public:
		ButtonControlFixture() : 
			option(15),
			disabledBackground("DisabledBG"),
			availableBackground("AvailableBG"),
			hotBackground("HotBG"),
			activeBackground("ActiveBG"),
			posX(10),
			posY(20),
			width(30),
			height(40),
			buttonControl(option, disabledBackground, availableBackground, hotBackground, activeBackground, posX, posY, width, height)
		{ }

		~ButtonControlFixture() {}

	void verifyBackgroundAndUpdateStatus(int mouseX, int mouseY, int buttonState, bool expectedUpdateReturn, std::string expectedSprite)
	{
		inputState.setMousePosition(mouseX, mouseY);
		inputState.setMouseButton(buttonState);

		CHECK_EQUAL(expectedUpdateReturn, buttonControl.update(inputState));

		IRenderCommand * renderCommand = buttonControl.draw();
		DrawSpriteCommand * drawSpriteCommand = dynamic_cast<DrawSpriteCommand *>(renderCommand);

		CHECK(0 != drawSpriteCommand);
		CHECK_EQUAL(expectedSprite, drawSpriteCommand->srcSpriteName);

		delete renderCommand;
	}

	public:
		Option option;
		std::string disabledBackground;
		std::string availableBackground;
		std::string hotBackground;
		std::string activeBackground;
		int posX;
		int posY;
		int width;
		int height;
		ButtonControl buttonControl;
		InputState inputState;
};

TEST_FIXTURE(ButtonControlFixture, ButtonControlIsActiveAndVisibleOnCreation)
{
	CHECK(buttonControl.isActive());
	CHECK(buttonControl.isVisible());
	CHECK(Point(posX, posY) == buttonControl.position());
	Rectangle dimensions = buttonControl.dimensions();
	CHECK_EQUAL(width, dimensions.width());
	CHECK_EQUAL(height, dimensions.height());
	CHECK(option == buttonControl.ID());
}

TEST_FIXTURE(ButtonControlFixture, ButtonControlShowsAppropriateBackgroundDependingOnInputState)
{
	verifyBackgroundAndUpdateStatus(0, 0, BUTTON_UP, false, availableBackground);
	verifyBackgroundAndUpdateStatus(0, 0, BUTTON_DOWN, false, availableBackground);
	verifyBackgroundAndUpdateStatus(11, 21, BUTTON_UP, false, hotBackground);
	verifyBackgroundAndUpdateStatus(11, 21, BUTTON_DOWN, false, activeBackground);
	verifyBackgroundAndUpdateStatus(11, 21, BUTTON_UP, true, hotBackground);
}
