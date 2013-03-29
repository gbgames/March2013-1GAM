#ifndef GB_ICOMMAND_FACTORY
#define GB_ICOMMAND_FACTORY

#include "Parameters.h"

#include <string>

class IGame;
class ICommand;
class ILevelInstance;
class IObjectFactory;

class ICommandFactory
{
	public:
		virtual ~ICommandFactory() {}
	public:
		virtual ICommand * createCommand(std::string cmdName, ILevelInstance *, Parameters parameters) = 0;
		virtual ICommand * createApplicationCommand(std::string cmdName, IGame &) = 0;
};

#endif
