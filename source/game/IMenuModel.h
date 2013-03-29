#ifndef GB_IMENUMODEL_H
#define GB_IMENUMODEL_H

#include "CommandIDs.h"
#include "Option.h"

namespace GBLib
{

class IMenuModel
{
	public:
		virtual ~IMenuModel() {}

	public:
		virtual unsigned int numOptions() const = 0;
		virtual Command::ID chooseOption(Option option) = 0;
};

}

#endif
