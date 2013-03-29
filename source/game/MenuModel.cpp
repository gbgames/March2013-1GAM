#include "MenuModel.h"

#include "CommandIDs.h"

using namespace GBLib;

MenuModel::MenuModel(Options options, Command::CommandIDs commands)
{
	m_optionsToCommands.clear();
	for (unsigned int index = 0; index < options.size(); ++index)
	{
		m_optionsToCommands.insert(std::pair<OptionID, Command::ID>(options.at(index).ID(), commands.at(index)));
	}
}

MenuModel::~MenuModel()
{
}

unsigned int MenuModel::numOptions() const
{
	return m_optionsToCommands.size();
}

Command::ID MenuModel::chooseOption(Option option)
{
	Command::ID commandID(Command::NO_COMMAND);

	std::map<OptionID, Command::ID>::iterator iter = m_optionsToCommands.find(option.ID());
	
	if (m_optionsToCommands.end() != iter)
	{
		commandID = iter->second;
	}

	return commandID;
}
