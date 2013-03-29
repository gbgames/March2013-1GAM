#ifndef GB_QUITAPPLICATIONCOMMAND_H
#define GB_QUITAPPLICATIONCOMMAND_H

#include "ICommand.h"

class IGame;

class QuitApplicationCommand : public ICommand
{
	public:
		QuitApplicationCommand(IGame & game);
		virtual ~QuitApplicationCommand();

	public:
		virtual void execute();

	public:
		IGame & m_game;
};

#endif
