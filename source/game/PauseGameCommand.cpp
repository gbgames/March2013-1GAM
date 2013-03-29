#include "PauseGameCommand.h"

#include "IGame.h"
#include "IStateManager.h"

PauseGameCommand::PauseGameCommand(IGame & game) :
	m_game(game)
{
}

PauseGameCommand::~PauseGameCommand()
{
}

void PauseGameCommand::execute()
{
	m_game.stateManager().pushState("PauseScreen");
}
