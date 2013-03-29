#ifndef GB_SPRITEMANAGER_H
#define GB_SPRITEMANAGER_H

#include "ISpriteManager.h"

#include <map>

class SpriteImage;

class SpriteManager : public ISpriteManager
{
	public:
		SpriteManager();
		virtual ~SpriteManager(); 

	public:
		virtual void createSpriteImage(std::string spriteName, std::string srcImage, Rectangle srcDimensions, Point hotSpot = Point(0, 0)); 
		virtual SpriteImage * getSprite(std::string spriteName);

	public:
		int numSprites();

	public:
		std::map<std::string, SpriteImage> m_nameToSpriteMapping;

};

#endif
