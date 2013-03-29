#include "StateManager.h"

#include "DrawSpriteCommand.h"
#include "IState.h"

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
	for (std::map<std::string, IState *>::iterator iter = m_nameToState.begin();
			iter != m_nameToState.end();
			++iter)
	{
		iter->second->onExit();
		delete iter->second;
	}
	m_nameToState.clear();
}

IState * StateManager::currentState()
{
	return getState(currentStateName());
}

std::string StateManager::currentStateName()
{
	std::string currentState("");
	if (!m_currentStates.empty())
	{
		currentState = m_currentStates.back();
	}

	return currentState;
}

void StateManager::registerState(std::string stateName, IState * state)
{
	m_nameToState[stateName] = state;
}

IState * StateManager::getState(std::string stateName)
{
	IState * state(0);
	if (m_nameToState.end() != m_nameToState.find(stateName))
	{
		state = m_nameToState[stateName];
	}

	return state;
}

void StateManager::initialize(std::string stateName)
{
	if (m_nameToState.end() != m_nameToState.find(stateName))
	{
		m_currentStates.clear();
		m_currentStates.push_back(stateName);
		currentState()->onEnter();
	}
}

void StateManager::pushState(std::string stateName)
{
	m_stateCommandQueue.push_back(new PushCommand(*this, stateName));
}

void StateManager::popState()
{
	m_stateCommandQueue.push_back(new PopCommand(*this));
}

void StateManager::clearTo(std::string stateName)
{
	m_stateCommandQueue.push_back(new ClearToCommand(*this, stateName));
}

void StateManager::update(int timeElapsed)
{
	for (std::vector<std::string>::iterator iter = m_currentStates.begin();
			iter != m_currentStates.end();
			++iter)
	{
		getState(*iter)->update(timeElapsed);
	}

	std::vector<StateCommand *>::iterator iter = m_stateCommandQueue.begin();
	while (iter != m_stateCommandQueue.end())
	{
		(*iter)->execute();
		delete *iter;
		iter = m_stateCommandQueue.erase(iter);
	}
	
}

IRenderCommand * StateManager::draw()
{
	DrawSpriteCommand * cmd = new DrawSpriteCommand();
	for (std::vector<std::string>::iterator iter = m_currentStates.begin();
			iter != m_currentStates.end();
			++iter)
	{
		cmd->add(getState(*iter)->draw());
	}

	return cmd;
}

StateManager::StateCommand::StateCommand(StateManager & stateManager) :
	m_stateManager(stateManager)
{
}

StateManager::ClearToCommand::ClearToCommand(StateManager & stateManager, std::string stateName) : 
	StateManager::StateCommand(stateManager),
	m_stateToClearTo(stateName)
{
}

StateManager::ClearToCommand::~ClearToCommand()
{
}

void StateManager::ClearToCommand::execute()
{
	if (m_stateManager.m_nameToState.end() != m_stateManager.m_nameToState.find(m_stateToClearTo))
	{
		while(!m_stateManager.m_currentStates.empty())
		{
			m_stateManager.currentState()->onExit();
			m_stateManager.m_currentStates.pop_back();
		}
		m_stateManager.initialize(m_stateToClearTo);
	}
}

StateManager::PushCommand::PushCommand(StateManager & stateManager, std::string stateName) : 
	StateManager::StateCommand(stateManager),
	m_stateToPush(stateName)
{
}

StateManager::PushCommand::~PushCommand()
{
}

void StateManager::PushCommand::execute()
{
	if (m_stateManager.m_nameToState.end() != m_stateManager.m_nameToState.find(m_stateToPush))
	{
		m_stateManager.currentState()->onSuspend();
		m_stateManager.m_currentStates.push_back(m_stateToPush);
		m_stateManager.currentState()->onEnter();
	}
}

StateManager::PopCommand::PopCommand(StateManager & stateManager) : 
	StateManager::StateCommand(stateManager)
{
}

StateManager::PopCommand::~PopCommand()
{
}

void StateManager::PopCommand::execute()
{
	if (!m_stateManager.m_currentStates.empty())
	{
		m_stateManager.currentState()->onExit();
		m_stateManager.m_currentStates.pop_back();
		m_stateManager.currentState()->onUnsuspend();
	}
}
