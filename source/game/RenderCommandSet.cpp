#include "RenderCommandSet.h"

#include "IHardwareLayer.h"
#include "IRenderCommand.h"

#include <algorithm>

struct IRenderCommand_less
		: std::binary_function<const IRenderCommand *, const IRenderCommand *, bool>
	{
		bool operator() (const IRenderCommand * lhs, const IRenderCommand * rhs ) const
		{
			return (*lhs < *rhs);
		}
	};

RenderCommandSet::RenderCommandSet() : commandSet(0)
{
}

RenderCommandSet::~RenderCommandSet()
{
	clear();
}

bool RenderCommandSet::empty() const
{
	return commandSet.empty();
}

int RenderCommandSet::size() const
{
	return commandSet.size();
}

void RenderCommandSet::add(IRenderCommand * cmd)
{
	if (0 != cmd)
	{
		commandSet.push_back(cmd);
	}
}

void RenderCommandSet::draw(IHardwareLayer & layer)
{
	std::sort(commandSet.begin(), commandSet.end(), IRenderCommand_less());

	for(std::vector<IRenderCommand *>::iterator iter = commandSet.begin();
			iter != commandSet.end();
			++iter)
	{
		(*iter)->draw(layer);
	}
}

void RenderCommandSet::clear()
{
	while (!commandSet.empty())
	{
		delete commandSet.back();
		commandSet.pop_back();
	}
}
