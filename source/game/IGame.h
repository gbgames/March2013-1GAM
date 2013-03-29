#ifndef GB_IGAME_H
#define GB_IGAME_H

class ICommandFactory;
class IHardwareLayer;
class InputState;
class IStateManager;
class RenderCommandSet;

class IGame
{
	public:
		virtual ~IGame()  {}

	public:
		virtual bool isDone() const = 0;
		virtual void quit() = 0;
		virtual void update(int timeElapsed) = 0;
		virtual int doUpdatesAndReturnLastTime(int lastTime, int now, int timeStep) = 0;
		virtual void draw() = 0;
		virtual void runWithFixedTimeStep(int timeStep) = 0;
		virtual void run(int timeStep, int now) = 0;
		virtual void populateRenderCommandSet() = 0;
		virtual RenderCommandSet & getRenderCommandSet() = 0;
		virtual IHardwareLayer & hardwareLayer() = 0;
		virtual InputState & getInputState() = 0; 
		virtual IStateManager & stateManager() = 0;
		virtual ICommandFactory & commandFactory() = 0;
};

#endif
