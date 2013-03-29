#ifndef GB_NULLCOMMAND_H
#define GB_NULLCOMMAND_H

#include "ICommand.h"

class NullCommand : public ICommand
{
	public:
		NullCommand();
		virtual ~NullCommand();
	
	public:
		virtual void execute();
};

#endif
