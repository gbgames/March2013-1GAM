#ifndef GB_MOUSERENDERINGSYSTEM_H
#define GB_MOUSERENDERINGSYSTEM_H

#include "IRenderingSystem.h"
#include "Point.h"

class InputState;
class ISpriteManager;

namespace GBLib
{

class MouseRenderingSystem : public IRenderingSystem
{
	public:
		MouseRenderingSystem(ISpriteManager & manager);
		virtual ~MouseRenderingSystem();
	
	public:
		virtual Option update(InputState & inputState);
		virtual IRenderCommand * draw();
	
	private:
		ISpriteManager & m_spriteManager;
		int m_hotID;
		int m_activeID;
		Point m_position;
};

}

#endif
