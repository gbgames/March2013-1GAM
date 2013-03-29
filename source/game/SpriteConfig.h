#ifndef GB_SPRITECONFIG_H
#define GB_SPRITECONFIG_H

#include "ISpriteConfig.h"

#include <map>

class SpriteConfig : public ISpriteConfig
{
	public:
		SpriteConfig();
		virtual ~SpriteConfig(); 

	public:
		virtual void initialize(IHardwareLayer & layer);
		virtual void addSprite(std::string spriteName, std::string filePath);
		virtual void addFilePrefix(std::string prefix);

	public:
		std::string m_pathFilePrefix;
		std::map<std::string, std::string> m_spriteImageToFilePath;
};

#endif
