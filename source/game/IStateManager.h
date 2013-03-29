#ifndef GB_ISTATEMANAGER_H
#define GB_ISTATEMANAGER_H

#include <string>

class IRenderCommand;
class IState;

class IStateManager
{
	public:
		virtual ~IStateManager() {}
	
	public:
		virtual IState * currentState() = 0;
		virtual std::string currentStateName() = 0;
		virtual void registerState(std::string stateName, IState * state) = 0;
		virtual IState * getState(std::string stateName) = 0;

		virtual void initialize(std::string stateName) = 0;
		virtual void pushState(std::string stateName) = 0;
		virtual void popState() = 0;
		virtual void clearTo(std::string stateName) = 0;

		virtual void update(int timeElapsed) = 0;
		virtual IRenderCommand * draw() = 0;

};

#endif
