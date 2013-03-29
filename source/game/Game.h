#ifndef GB_GAME_H
#define GB_GAME_H

#include "IGame.h"
#include "InputState.h"
#include "RenderCommandSet.h"

#include <sstream>

class Game : public IGame
{
	public:
		Game(IHardwareLayer & hardwareLayer, 
			IStateManager & stateManager,
			ICommandFactory & commandFactory);
		virtual ~Game();

	public:
		virtual bool isDone() const;
		virtual void quit();
		virtual void initialize();
		virtual void update(int timeElapsed);
		virtual int doUpdatesAndReturnLastTime(int lastTime, int now, int timeStep);
		virtual void draw();
		virtual void populateRenderCommandSet();
		virtual void runWithFixedTimeStep(int timeStep);
		virtual void run(int timeStep, int now);
		virtual RenderCommandSet & getRenderCommandSet();
		virtual IHardwareLayer & hardwareLayer();
		virtual InputState & getInputState();
		virtual IStateManager & stateManager();
		virtual ICommandFactory & commandFactory();

	protected:
		bool m_isDone;
		RenderCommandSet m_renderCommandSet;

		IHardwareLayer & m_hardwareLayer;
		InputState m_inputState;
		IStateManager & m_stateManager;
		ICommandFactory & m_commandFactory;

		std::stringstream debugOutput;
		bool m_shouldDraw;
};

#endif
