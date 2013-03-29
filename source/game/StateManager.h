#ifndef GB_STATEMANAGER_H
#define GB_STATEMANAGER_H

#include "IStateManager.h"

#include "ICommand.h"

#include <map>
#include <vector>

class StateManager : public IStateManager
{
	public:
		StateManager();
		virtual ~StateManager();
	
	public:
		virtual IState * currentState();
		virtual std::string currentStateName();
		virtual void registerState(std::string stateName, IState * state);
		virtual IState * getState(std::string stateName);

		virtual void initialize(std::string stateName);
		virtual void pushState(std::string stateName);
		virtual void popState();
		virtual void clearTo(std::string stateName);

		virtual void update(int timeElapsed);
		virtual IRenderCommand * draw();

	public:
		class StateCommand : public ICommand
		{
			public:	
				StateCommand(StateManager & stateManager);
				virtual ~StateCommand() {}

			public:
				StateManager & m_stateManager;
		};

		class ClearToCommand : public StateCommand
		{
			public:
				ClearToCommand(StateManager & stateManager, std::string stateName); 
				virtual ~ClearToCommand();

			public:
				virtual void execute();

			public:
				std::string m_stateToClearTo;
		};

		class PushCommand : public StateCommand
		{
			public:
				PushCommand(StateManager & stateManager, std::string stateName); 
				virtual ~PushCommand();

			public:
				virtual void execute();

			public:
				std::string m_stateToPush;
		};

		class PopCommand : public StateCommand
		{
			public:
				PopCommand(StateManager & stateManager); 
				virtual ~PopCommand();

			public:
				virtual void execute();
		};

		std::map<std::string, IState *> m_nameToState;
		std::vector<std::string> m_currentStates;
		std::vector<StateCommand *> m_stateCommandQueue;
};

#endif
