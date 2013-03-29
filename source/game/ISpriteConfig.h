#ifndef GB_ISPRITECONFIG_H
#define GB_ISPRITECONFIG_H

class IHardwareLayer;

#include <string>

class ISpriteConfig
{
	public:
		virtual ~ISpriteConfig() {}

	public:
		virtual void initialize(IHardwareLayer & layer) = 0;
		virtual void addSprite(std::string spriteName, std::string filePath) = 0;
		virtual void addFilePrefix(std::string prefix) = 0;
};

#endif
