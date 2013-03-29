#ifndef GB_MOCKSPRITEIMAGE_H
#define GB_MOCKSPRITEIMAGE_H

#include "DrawSpriteCommand.h"
#include "SpriteImage.h"


class MockSpriteImage : public SpriteImage
{
	public:
		MockSpriteImage() : SpriteImage(), m_drawCalledWithPoint(999, 999), m_drawCalledWithViewPort(0) {}
		virtual ~MockSpriteImage() {}

		virtual IRenderCommand * draw(Point p, IViewPort * viewPort) { m_drawCalledWithPoint = p; m_drawCalledWithViewPort = viewPort; return new DrawSpriteCommand(); }

		Point m_drawCalledWithPoint;
		IViewPort * m_drawCalledWithViewPort;

};

#endif
