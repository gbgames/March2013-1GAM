#ifndef GB_RENDERCOMMANDSET_H
#define GB_RENDERCOMMANDSET_H

#include <vector>

class IHardwareLayer;
class IRenderCommand;

class RenderCommandSet
{
	public:
		RenderCommandSet();
		~RenderCommandSet();

	public:
		bool empty() const;
		int size() const;
		void add(IRenderCommand * cmd); 
		void draw(IHardwareLayer & layer);
		void clear();

	public:
		std::vector<IRenderCommand *> commandSet;
};

#endif
