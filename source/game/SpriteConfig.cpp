#include "SpriteConfig.h"

#include "IHardwareLayer.h"

SpriteConfig::SpriteConfig()
{
}

SpriteConfig::~SpriteConfig()
{
}

void SpriteConfig::initialize(IHardwareLayer & layer)
{
	for (std::map<std::string, std::string>::iterator iter = m_spriteImageToFilePath.begin();
				iter != m_spriteImageToFilePath.end();
				++iter)
	{
		layer.loadSprite(iter->first, iter->second);
	}
}

void SpriteConfig::addSprite(std::string spriteName, std::string filePath)
{
	m_spriteImageToFilePath[spriteName] = filePath;
}

void SpriteConfig::addFilePrefix(std::string prefix)
{
	m_pathFilePrefix = prefix;
}
