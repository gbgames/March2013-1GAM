#ifndef GB_ICOMMAND_H
#define GB_ICOMMAND_H

#include <vector>

class ICommand
{
	public:
		virtual ~ICommand() {}
	
	public:
		virtual void execute() = 0;
};

#endif
