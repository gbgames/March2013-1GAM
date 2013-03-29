#ifndef GB_MENUMODEL_H
#define GB_MENUMODEL_H

#include "IMenuModel.h"

#include <map>

namespace GBLib
{

class MenuModel : public IMenuModel
{
	public:
		MenuModel(Options options, Command::CommandIDs commands);
		virtual ~MenuModel();
	
	public:
		virtual unsigned int numOptions() const;
		virtual Command::ID chooseOption(Option option);

	public:
		std::map<OptionID, Command::ID> m_optionsToCommands;
};

}

#endif
