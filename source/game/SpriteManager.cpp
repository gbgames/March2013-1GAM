#include "SpriteManager.h"

#include "SpriteImage.h"

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
}

void SpriteManager::createSpriteImage(std::string spriteName, std::string srcImage, Rectangle srcDimensions, Point hotSpot)
{
	SpriteImage sprite(srcImage, srcDimensions, hotSpot);
	m_nameToSpriteMapping[spriteName] = sprite;
}

SpriteImage * SpriteManager::getSprite(std::string spriteName)
{
	SpriteImage * sprite(0);

	std::map<std::string, SpriteImage>::iterator spriteIter = m_nameToSpriteMapping.find(spriteName);
	if (spriteIter != m_nameToSpriteMapping.end())
	{
		sprite = &m_nameToSpriteMapping[spriteName];
	}

	return sprite;
}

int SpriteManager::numSprites()
{
	return m_nameToSpriteMapping.size();
}
