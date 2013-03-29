#include "Game.h"

#include "IHardwareLayer.h"

#include <iostream>

Game::Game(IHardwareLayer & hardwareLayer, 
			IStateManager & stateManager,
			ICommandFactory & commandFactory) : 
	m_isDone(false),
	m_hardwareLayer(hardwareLayer),
	m_inputState(&m_hardwareLayer.getKeyboardState()),
	m_stateManager(stateManager),
	m_commandFactory(commandFactory),
	m_shouldDraw(true)
{
	debugOutput.clear();
}

Game::~Game()
{
}

bool Game::isDone() const
{ 
	return m_isDone; 
}

void Game::quit() 
{
	m_isDone = true;
	//std::cout << debugOutput.str() << std::endl;
}

void Game::initialize()
{
	m_hardwareLayer.setScreen("A GBGames Game", 800, 600, 24);
	m_hardwareLayer.setAudio(44100, 2, 1024);
	m_hardwareLayer.initialize();
}

void Game::update(int timeElapsed)
{
	timeElapsed = timeElapsed;
	m_isDone = true;
}

int Game::doUpdatesAndReturnLastTime(int lastTime, int now, int timeStep)
{
	debugOutput << "========= DO UPDATES AND RETURN LAST TIME CALLED at " << now << "=====\n";

	int whichUpdate(0);

	const int MAX_TIME_STEP = 250;
	if (now - lastTime > MAX_TIME_STEP)
	{
		lastTime = now - MAX_TIME_STEP;
	}

	while (lastTime + timeStep <= now)
	{
		m_shouldDraw = true;
		debugOutput << "-----Update # " << whichUpdate++ << " started at: " << m_hardwareLayer.getTicks();
		debugOutput << "\n";

			update(timeStep);
			lastTime += timeStep;

		debugOutput << "-----Update ended at: " << m_hardwareLayer.getTicks();
		debugOutput << "\n";
	}

	return lastTime;
}

void Game::draw()
{
	debugOutput << "========= DRAW CALLED at " << m_hardwareLayer.getTicks() << "=====\n";
	m_renderCommandSet.clear();
	populateRenderCommandSet();
	m_renderCommandSet.draw(m_hardwareLayer);
	m_hardwareLayer.render();
	debugOutput << "========= DRAW ended at " << m_hardwareLayer.getTicks() << "=====\n";
}

void Game::populateRenderCommandSet()
{
}

void Game::runWithFixedTimeStep(int timeStep)
{
	int now = m_hardwareLayer.getTicks();
	run(timeStep, now);
}

void Game::run(int timeStep, int firstTime)
{
	int lastTime = firstTime;
	while(!isDone())
	{
		int now = m_hardwareLayer.getTicks();

		lastTime = doUpdatesAndReturnLastTime(lastTime, now, timeStep);

		if (m_shouldDraw)
		{
			draw();
			m_shouldDraw = false;
		}

		m_hardwareLayer.delay(1);
	}
}

RenderCommandSet & Game::getRenderCommandSet()
{
	return m_renderCommandSet;
}

IHardwareLayer & Game::hardwareLayer()
{
	return m_hardwareLayer;
}

InputState & Game::getInputState()
{
	return m_inputState;
}

IStateManager & Game::stateManager()
{
	return m_stateManager;
}

ICommandFactory & Game::commandFactory()
{
	return m_commandFactory;
}
