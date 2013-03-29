#ifndef GB_IRENDERINGSYSTEM_H
#define GB_IRENDERINGSYSTEM_H

#include "Option.h"

class InputState;
class IRenderCommand;

namespace GBLib
{

class IRenderingSystem
{
	public:
		virtual ~IRenderingSystem() {}
	
	public:
		virtual Option update(InputState & inputState) = 0;
		virtual IRenderCommand * draw() = 0;
};

}

#endif
