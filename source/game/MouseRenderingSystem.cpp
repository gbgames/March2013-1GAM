#include "MouseRenderingSystem.h"

#include "InputState.h"
#include "ISpriteManager.h"
#include "Point.h"
#include "SpriteImage.h"
#include "ZOrder.h"
#include <string>

using namespace GBLib;

MouseRenderingSystem::MouseRenderingSystem(ISpriteManager & spriteManager) :
	m_spriteManager(spriteManager),
	m_position(0, 0)
{
}

MouseRenderingSystem::~MouseRenderingSystem()
{
}


Option MouseRenderingSystem::update(InputState & inputState)
{
	m_hotID = inputState.getHotID();
	m_activeID = inputState.getActiveID();
	m_position = inputState.position();
}

IRenderCommand * MouseRenderingSystem::draw()
{
	std::string mouseCursor("MouseCursor-BasicSprite");
	if (BID_NONE != m_hotID)
	{
		mouseCursor = "MouseCursor-HotSprite";
	}
	if (BID_NONE != m_activeID)
	{
		mouseCursor = "MouseCursor-ActiveSprite";
	}
	Point mousePosition(m_position.intX(), m_position.intY(), ZORDER::MOUSE_CURSOR);
	SpriteImage * sprite = m_spriteManager.getSprite(mouseCursor);

	IRenderCommand * cmd(0);
	if (sprite)
	{
		cmd = sprite->draw(mousePosition);
	}

	return cmd;
}
