#include "MainMenuState.h"

#include "DrawSpriteCommand.h"
#include "IGame.h"
#include "IHardwareLayer.h"
#include "ISpriteConfig.h"
#include "IStateManager.h"
#include "MenuModel.h"
#include "MenuView.h"
#include "MouseRenderingSystem.h"
#include "Option.h"
#include "SpriteImage.h"
#include "ViewConfiguration.h"

#include <sstream>
#include <vector>

using namespace GBLib;

MainMenuState::MainMenuState(IGame & game, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager) :
	m_game(game),
	m_spriteConfig(spriteConfig),
	m_spriteManager(spriteManager),
	m_splashScreenSprite(0),
	m_stateSuspended(false),
	m_menuModel(0),
	m_viewConfiguration(0),
	m_menuView(0),
	m_mouseRenderer(0)
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::onEnter()
{
	Options options;
	options.push_back(Option(START_GAME));
	options.push_back(Option(QUIT_GAME));

	Command::CommandIDs commands;
	commands.push_back(Command::NEW_GAME);
	commands.push_back(Command::QUIT_APPLICATION);

	delete m_menuModel;
	m_menuModel = new MenuModel(options, commands);

	delete m_viewConfiguration;
	m_viewConfiguration = new ViewConfiguration(*m_menuModel);
	m_viewConfiguration->registerButtonControl(options.at(0), "BasicBlockImage", "BasicBlockImage", "SemiLitBlockImage", "LitBlockImage", Rectangle(200, 356, 64, 64));
	m_viewConfiguration->registerButtonControl(options.at(1), "BasicBlockImage", "BasicBlockImage", "SemiLitBlockImage", "LitBlockImage", Rectangle(800 - 200 - 64, 356, 64, 64));

	delete m_menuView;
	m_menuView = new MenuView(*m_menuModel, *m_viewConfiguration);

	delete m_mouseRenderer;
	m_mouseRenderer = new MouseRenderingSystem(m_spriteManager);
}

void MainMenuState::update(int timeElapsed)
{
	Command::ID commandID(Command::NO_COMMAND); 

	Option option = m_menuView->update(m_game.getInputState());
	if (Option::NO_OPTION != option.ID())
	{
		commandID = m_menuModel->chooseOption(option);
	}

	switch(commandID)
	{
		case Command::QUIT_APPLICATION: 
		{
			m_game.stateManager().clearTo("ExitScreen");
		}
		break;

		case Command::NEW_GAME:
		{
			m_game.stateManager().clearTo("InGame");
		}
		break;

		case Command::NO_COMMAND:
		default:
		break;
	}

	m_mouseRenderer->update(m_game.getInputState());
}

IRenderCommand * MainMenuState::draw()
{
	DrawSpriteCommand * cmd = new DrawSpriteCommand();
	cmd->setDrawComplete("MenuScreenBackgroundImage", Point(0, 0));
	DrawSpriteCommand * titleScreen = new DrawSpriteCommand();
	titleScreen->setDrawComplete("TitleScreenImage", Point(0, 0));
	cmd->add(titleScreen);
	titleScreen->add(m_menuView->draw());
	titleScreen->add(m_mouseRenderer->draw());

	return cmd;
}

void MainMenuState::onExit()
{
	delete m_mouseRenderer;
	m_mouseRenderer = 0;
	delete m_menuView;
	m_menuView = 0;
	delete m_viewConfiguration;
	m_viewConfiguration = 0;
	delete m_menuModel;
	m_menuModel = 0;
}

void MainMenuState::onSuspend()
{
	m_stateSuspended = true;
}

void MainMenuState::onUnsuspend()
{
	m_stateSuspended = false;
}
