#include "UnitTest++.h"

#include "Option.h"
#include "CommandIDs.h"
#include "DrawSpriteCommand.h"
#include "DrawTextCommand.h"
#include "InputState.h"
#include "MenuModel.h"
#include "MenuView.h"
#include "RenderCommandSet.h"
#include "ViewConfiguration.h"

#include <iostream>

using namespace GBLib;

class MenuViewFixture
{
	public:
		MenuViewFixture() :
			OPTION_1(1),
			disabledImage1("DISABLED_1"),
			activeImage1("ACTIVE_1"),
			hotImage1("HOT_1"),
			availableImage1("AVAILABLE_1"),
			dimensions1(10, 10, 20, 20),
			text("FooText"),
			textRenderPosition(11, 22, 33),
			fontName("FooFont"),
			red(13),
			green(14),
			blue(15),
			hotSpot(0.8, 0.7),
			renderCommand(0)
	{
	}

		~MenuViewFixture()
		{
			delete view;
			delete viewConfiguration;
			delete menuModel;
			delete renderCommand;
		}

	public:
		void configureTextLabelsInMenuView()
		{
			menuModel = new MenuModel(options, commands);
			viewConfiguration = new ViewConfiguration(*menuModel);
			viewConfiguration->registerText(text, textRenderPosition, fontName, red, green, blue, hotSpot);
			view = new MenuView(*menuModel, *viewConfiguration);
		}

		void configureButtonControlsInMenuView()
		{
			options.push_back(Option(OPTION_1));

			commands.push_back(Command::QUIT_APPLICATION);

			menuModel = new MenuModel(options, commands);
			viewConfiguration = new ViewConfiguration(*menuModel);
			viewConfiguration->registerButtonControl(OPTION_1, disabledImage1, availableImage1, hotImage1, activeImage1, dimensions1);
			view = new MenuView(*menuModel, *viewConfiguration);
		}

		void verifyCorrectImageIsDrawn(std::string imageID)
		{
			DrawSpriteCommand * drawMenuCommand = dynamic_cast<DrawSpriteCommand *>(renderCommand);
			DrawSpriteCommand * drawCommand = dynamic_cast<DrawSpriteCommand *>(drawMenuCommand->renderSet.commandSet.at(0));

			CHECK_EQUAL(imageID, drawCommand->srcSpriteName);
		}

		void verifyCorrectTextIsRendered(std::string sampleText)
		{
			DrawSpriteCommand * drawMenuCommand = dynamic_cast<DrawSpriteCommand *>(renderCommand);
			DrawTextCommand * drawTextCommand = dynamic_cast<DrawTextCommand *>(drawMenuCommand->renderSet.commandSet.at(0));
			CHECK_EQUAL(sampleText, drawTextCommand->m_textToRender);
		}

	public:
		Option OPTION_1;
		Options options;
		Command::CommandIDs commands;
		std::string disabledImage1;
		std::string activeImage1;
		std::string hotImage1;
		std::string availableImage1;
		Rectangle dimensions1;
		std::string text;
		Point textRenderPosition;
		std::string fontName;
		int red, green, blue;
		Point hotSpot;
		InputState inputState;
		IRenderCommand * renderCommand;
		MenuModel * menuModel;
		ViewConfiguration * viewConfiguration;
		MenuView * view;
};

TEST_FIXTURE(MenuViewFixture, MenuViewShowsTextLabels)
{
	configureTextLabelsInMenuView();
	renderCommand = view->draw();
	verifyCorrectTextIsRendered(text);
}

TEST_FIXTURE(MenuViewFixture, MenuViewShowsAvailableImageIfButtonIsNotMousedOver)
{
	configureButtonControlsInMenuView();
	inputState.setMousePosition(0, 0);
	inputState.setMouseButton(BUTTON_UP);

	Option chosenOption = view->update(inputState);

	CHECK_EQUAL(Option::NO_OPTION, chosenOption.ID());

	renderCommand = view->draw();

	verifyCorrectImageIsDrawn(availableImage1);
}

TEST_FIXTURE(MenuViewFixture, MenuViewShowsHotImageIfButtonIsMousedOver)
{
	configureButtonControlsInMenuView();
	inputState.setMousePosition(11, 11);
	inputState.setMouseButton(BUTTON_UP);

	Option chosenOption = view->update(inputState);

	CHECK_EQUAL(Option::NO_OPTION, chosenOption.ID());

	renderCommand = view->draw();

	verifyCorrectImageIsDrawn(hotImage1);
}

TEST_FIXTURE(MenuViewFixture, MenuViewShowsActiveImageIfButtonIsMousedOverAndDown)
{
	configureButtonControlsInMenuView();
	inputState.setMousePosition(11, 11);
	inputState.setMouseButton(BUTTON_DOWN);

	Option chosenOption = view->update(inputState);

	CHECK_EQUAL(Option::NO_OPTION, chosenOption.ID());

	renderCommand = view->draw();

	verifyCorrectImageIsDrawn(activeImage1);
}

TEST_FIXTURE(MenuViewFixture, MenuViewReturnsOptionIDOfButtonIfClickedAndReleased)
{
	configureButtonControlsInMenuView();
	inputState.setMousePosition(11, 11);
	inputState.setMouseButton(BUTTON_DOWN);

	Option chosenOption = view->update(inputState);

	inputState.setMouseButton(BUTTON_UP);
	
	chosenOption = view->update(inputState);

	CHECK_EQUAL(OPTION_1.ID(), chosenOption.ID());

	renderCommand = view->draw();

	verifyCorrectImageIsDrawn(hotImage1);
}
