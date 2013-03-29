#include "OneGAMGame.h"

#include "ICommand.h"
#include "ICommandFactory.h"
#include "IHardwareLayer.h"
#include "IStateManager.h"

OneGAMGame::OneGAMGame(IHardwareLayer & hardwareLayer, 
			IStateManager & stateManager,
			ICommandFactory & commandFactory) : 
			Game(hardwareLayer, stateManager, commandFactory)
{
}

OneGAMGame::~OneGAMGame()
{
}

void OneGAMGame::update(int timeElapsed)
{
	prepareInputState();

	updateInput();

	m_stateManager.update(timeElapsed);

	finishInputState();
}

void OneGAMGame::initialize()
{
	m_stateManager.initialize("Startup");
}

void OneGAMGame::populateRenderCommandSet()
{
	m_renderCommandSet.add(m_stateManager.draw());
}

void OneGAMGame::updateInput()
{
	m_hardwareLayer.update();
	if (m_hardwareLayer.hasQuitEventOccurred())
	{
		m_hardwareLayer.resetQuitStatus();
		ICommand * command = commandFactory().createApplicationCommand("Quit", *this);
		command->execute();
		delete command;
	}
	m_inputState.setMousePosition(m_hardwareLayer.getMousePosition().X(), m_hardwareLayer.getMousePosition().Y());
	m_inputState.setMouseButton(m_hardwareLayer.getMouseButtonStatus());

}

void OneGAMGame::prepareInputState()
{
	m_inputState.setHot(BID_NONE);
}

void OneGAMGame::finishInputState()
{
	if (BUTTON_UP == m_inputState.mouseButtonStatus())
	{
		m_inputState.setActive(BID_NONE);
	}
	else
	{
		if (BID_NONE == m_inputState.getActiveID())
		{
			m_inputState.setActive(BID_UNAVAILABLE);
		}
	}
}
