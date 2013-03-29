#ifndef GB_ISTATE_H
#define GB_ISTATE_H

class IRenderCommand;

class IState
{
	public:
		virtual ~IState() {}
	
	public:
		virtual void onEnter() = 0;
		virtual void update(int timeElapsed) = 0;
		virtual IRenderCommand * draw() = 0; 
		virtual void onExit() = 0; 

		virtual void onSuspend() = 0;
		virtual void onUnsuspend()  = 0;
};

#endif
