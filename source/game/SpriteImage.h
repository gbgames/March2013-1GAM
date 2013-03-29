#ifndef GB_SPRITEIMAGE_H
#define GB_SPRITEIMAGE_H

#include "Point.h"
#include "Rectangle.h"

#include <string>

class IRenderCommand;
class IViewPort;

class SpriteImage
{
	public:
		SpriteImage();
		SpriteImage(std::string imageName, Rectangle srcRect, Point hotSpot = Point(0, 0));
		virtual ~SpriteImage();
		SpriteImage & operator=(const SpriteImage & rhs);

	public:
		virtual Point hotSpot() const;
		virtual std::string spriteImage() const;
		virtual Rectangle spriteImageRect() const;

		virtual void setHotSpot(Point hotSpot);
		virtual void setImage(std::string imageName, Rectangle srcRect);

		virtual IRenderCommand * draw(Point position, IViewPort * viewPort = 0);

	public:
		Point m_hotSpot;
		std::string m_imageName;
		Rectangle m_imageRect;
};

#endif
