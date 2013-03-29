#ifndef GB_COMMAND_FACTORY
#define GB_COMMAND_FACTORY

#include "ICommand.h"
#include "ICommandFactory.h"

#include "CommandIDs.h"

#include <map>

class IObjectFactory;

class CommandFactory : public ICommandFactory
{
	public:
		CommandFactory(IObjectFactory & objectFactory); 
		virtual ~CommandFactory();

	public:
		virtual ICommand * createCommand(std::string cmdName, ILevelInstance *, Parameters parameters);
		virtual ICommand * createApplicationCommand(std::string cmdName, IGame & game);
	
	public:
		IObjectFactory & m_objectFactory;
		std::map<std::string, Command::ID> m_commandNameToID;
};

#endif
