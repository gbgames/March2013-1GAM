#include "ExitScreenState.h"

#include "DrawSpriteCommand.h"
#include "ICommand.h"
#include "ICommandFactory.h"
#include "IGame.h"
#include "IHardwareLayer.h"
#include "ISpriteConfig.h"
#include "IStateManager.h"
#include "IVersionInfo.h"
#include "SpriteImage.h"

ExitScreenState::ExitScreenState(IGame & game, ISpriteManager & spriteManager, IVersionInfo & versionInfo):
	m_game(game),
	m_spriteManager(spriteManager),
	m_versionInfo(versionInfo),
	m_exitScreenSprite(0),
	m_hasRendered(false),
	m_stateSuspended(false),
	m_hasShownDemoSalesPage(false)
{
}

ExitScreenState::~ExitScreenState()
{
}

void ExitScreenState::onEnter()
{
	m_spriteManager.createSpriteImage("ExitScreenSprite", "ExitScreenImage", Rectangle(0, 0, 800, 600));
	m_exitScreenSprite = m_spriteManager.getSprite("ExitScreenSprite");
}

void ExitScreenState::update(int timeElapsed)
{
	if (!m_stateSuspended)
	{
		if (Version::DEMO == m_versionInfo.demoStatus())
		{
			if (!m_hasShownDemoSalesPage)
			{
				m_game.stateManager().pushState("SalesScreen");
			}
		}
		else
		{
			m_hasShownDemoSalesPage = true;
		}

		if (m_hasShownDemoSalesPage)
		{
			if (m_hasRendered)
			{
				m_game.quit();
			}
		}
	}
}

IRenderCommand * ExitScreenState::draw()
{
	IRenderCommand * command(0);
	if (!m_stateSuspended)
	{
		m_hasRendered = true; 
		DrawSpriteCommand * cmd = new DrawSpriteCommand();
		command = cmd;
		cmd->setDrawComplete("MenuScreenBackgroundImage", Point(0, 0));
		cmd->add(m_exitScreenSprite->draw(Point(0, 0))); 
	}

	return command;
}

void ExitScreenState::onExit()
{
}

void ExitScreenState::onSuspend()
{
	m_stateSuspended = true;
}

void ExitScreenState::onUnsuspend()
{
	m_stateSuspended = false;
	m_hasShownDemoSalesPage = true;
}
