#include "UnitTest++.h"

#include "MouseRenderingSystem.h"

#include "InputState.h"
#include "MockSpriteImage.h"
#include "MockSpriteManager.h"
#include "SpriteImage.h"
#include "ZOrder.h"

using namespace GBLib;

struct MouseRenderingSystemFixture
{
	MockSpriteManager spriteManager;
	MockSpriteImage * sprite; 
	Point position;
	InputState inputState;
	MouseRenderingSystem r;
	IRenderCommand * command;

	MouseRenderingSystemFixture() :
		sprite(new MockSpriteImage()),
		position(50, 60, ZORDER::MOUSE_CURSOR),
		inputState(position.intX(), position.intY(), BUTTON_UP),
		r(spriteManager),
		command(0)
	{
		spriteManager.setSprite(sprite);
	}

	~MouseRenderingSystemFixture() 
	{
		delete command;
		delete sprite;
	}
};

TEST_FIXTURE(MouseRenderingSystemFixture, MouseRenderingSystemRendersMousePointer)
{
	r.update(inputState);
	command = r.draw();

	CHECK(spriteManager.verifyGetSpriteCalledWith("MouseCursor-BasicSprite"));
	CHECK(position == sprite->m_drawCalledWithPoint);
}

TEST_FIXTURE(MouseRenderingSystemFixture, MouseRenderingSystemRendersHotMousePointer)
{
	inputState.setHot(1);

	r.update(inputState);
	command = r.draw();

	CHECK(spriteManager.verifyGetSpriteCalledWith("MouseCursor-HotSprite"));
	CHECK(position == sprite->m_drawCalledWithPoint);
}

TEST_FIXTURE(MouseRenderingSystemFixture, MouseRenderingSystemRendersActiveMousePointer)
{
	inputState.setHot(1);
	inputState.setActive(1);

	r.update(inputState);
	command = r.draw();

	CHECK(spriteManager.verifyGetSpriteCalledWith("MouseCursor-ActiveSprite"));
	CHECK(position == sprite->m_drawCalledWithPoint);
}
