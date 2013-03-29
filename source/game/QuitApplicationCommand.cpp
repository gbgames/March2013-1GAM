#include "QuitApplicationCommand.h"

#include "IGame.h"
#include "IHardwareLayer.h"
#include "IStateManager.h"

QuitApplicationCommand::QuitApplicationCommand(IGame & game) :
	m_game(game)
{
}

QuitApplicationCommand::~QuitApplicationCommand()
{
}

void QuitApplicationCommand::execute()
{
	std::string currentState = m_game.stateManager().currentStateName();
	if ("SalesScreen" == currentState)
	{
		m_game.stateManager().popState();
	} 
	else if("ExitScreen" != currentState )
	{
		m_game.stateManager().clearTo("ExitScreen");
	}
}
