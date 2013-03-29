#ifndef GB_PAUSEGAMECOMMAND_H
#define GB_PAUSEGAMECOMMAND_H

#include "ICommand.h"

class IGame;

class PauseGameCommand : public ICommand
{
	public:
		PauseGameCommand(IGame & game);
		virtual ~PauseGameCommand();

	public:
		virtual void execute();

	public:
		IGame & m_game;
};

#endif
