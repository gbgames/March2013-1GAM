#ifndef GB_MOCKSPRITEMANAGER_H
#define GB_MOCKSPRITEMANAGER_H

#include "ISpriteManager.h"

#include "SpriteImage.h"

#include <algorithm>

class MockSpriteManager : public ISpriteManager
{
	public:
		MockSpriteManager() : 
			m_createSpriteName(""),
			m_createSpriteCalled(""),
			m_createSpriteOffset(999, 999),
			m_createSpriteRect(999, 999, 999, 999),
			m_sprite(0)
		{}
		virtual ~MockSpriteManager() {}
		
		virtual void createSpriteImage(std::string spriteName, std::string srcImage, Rectangle srcDimensions, Point hotSpot = Point(0, 0)) {
			m_createSpriteName = spriteName;
			m_createSpriteCalled = srcImage;
			m_createSpriteOffset = Point(0, 0);
			m_createSpriteRect = srcDimensions;
		}

		virtual SpriteImage * getSprite(std::string spriteName) {
			m_getSpriteCalled.push_back(spriteName);
			return m_sprite;
		}

	public:
		void setSprite(SpriteImage * sprite) {
			m_sprite = sprite;
		}

		bool verifyGetSpriteCalledWith(std::string spriteName) {
			return m_getSpriteCalled.end() != std::find(m_getSpriteCalled.begin(), m_getSpriteCalled.end(), spriteName);
		}

	public:
		std::string m_createSpriteName;
		std::string m_createSpriteCalled;
		Point m_createSpriteOffset;
		Rectangle m_createSpriteRect;

		std::vector<std::string> m_getSpriteCalled;
		SpriteImage * m_sprite;
};

#endif
