#ifndef GB_ISPRITEMANAGER_H
#define GB_ISPRITEMANAGER_H

#include "Point.h"
#include "Rectangle.h"

#include <string>

class SpriteImage;

class ISpriteManager
{
	public:
		virtual ~ISpriteManager() {}

		virtual void createSpriteImage(std::string spriteName, std::string srcImage, Rectangle srcDimensions, Point hotSpot = Point(0, 0)) = 0;
		virtual SpriteImage * getSprite(std::string spriteName) = 0;
};

#endif
