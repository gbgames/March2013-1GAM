#ifndef GB_IRENDERCOMMAND_H
#define GB_IRENDERCOMMAND_H

#include "Point.h"

class IHardwareLayer; 

class IRenderCommand
{
	public:
		IRenderCommand(float x, float y, float z = 0) :
			dstPosition(x, y, z) {}
		virtual ~IRenderCommand() {}

	public:
		virtual void draw(IHardwareLayer & layer) =0;

		virtual void add(IRenderCommand *) = 0;
		
		bool operator<(const IRenderCommand &cmd) const { 
			bool isLessThan(this->dstPosition < cmd.dstPosition);

			return isLessThan;
		}

	public:
		Point dstPosition;
};

#endif
