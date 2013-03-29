#ifndef GB_ONEGAMGAME_H
#define GB_ONEGAMGAME_H

#include "Game.h"

class OneGAMGame : public Game
{
	public:
		OneGAMGame(IHardwareLayer & hardwareLayer, 
			IStateManager & stateManager,
			ICommandFactory & commandFactory); 
		virtual ~OneGAMGame();
	
	public:
		virtual void update(int timeElapsed);
		virtual void initialize();
		virtual void populateRenderCommandSet();

		void updateInput();

	private:
		void prepareInputState();
		void finishInputState();
};

#endif
