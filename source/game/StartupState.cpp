#include "StartupState.h"

#include "IGame.h"
#include "IHardwareLayer.h"
#include "ISpriteConfig.h"
#include "ISpriteManager.h"
#include "IStateManager.h"
#include "SpriteImage.h"

StartupState::StartupState(IGame & game, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager) : 
	m_game(game),
	m_spriteConfig(spriteConfig),
	m_spriteManager(spriteManager),
	m_splashScreenSprite(0),
	m_tickCountdown(0)
{
}

StartupState::~StartupState()
{
}

void StartupState::onEnter()
{
	//TODO: Make this configurable/dynamic?
	m_game.hardwareLayer().setScreen("March 1GAM  by GBGames", 800, 600, 24);
	m_game.hardwareLayer().setAudio(44100, 2, 1024);
	m_game.hardwareLayer().initialize();

	configureSplashScreenSpriteConfig();
	m_spriteConfig.initialize(m_game.hardwareLayer());

	m_spriteManager.createSpriteImage("SplashScreenSprite", "SplashScreenImage", Rectangle(0, 0, 800, 600));
	m_splashScreenSprite = m_spriteManager.getSprite("SplashScreenSprite");

	m_tickCountdown = 100;
}

void StartupState::update(int timeElapsed)
{
	m_tickCountdown -= timeElapsed;
	if (m_tickCountdown <= 0)
	{
		m_game.stateManager().clearTo("MediaLoad");
	}
}

IRenderCommand * StartupState::draw()
{
	return m_splashScreenSprite->draw(Point(0, 0)); 
}

void StartupState::onExit()
{
	//TODO: Get rid of the Splash Screen sprite. It's not needed anymore.
}

void StartupState::onSuspend()
{
}

void StartupState::onUnsuspend()
{
}

void StartupState::configureSplashScreenSpriteConfig()
{
	m_spriteConfig.addSprite("SplashScreenImage", "resources/SplashScreen.png");
}
