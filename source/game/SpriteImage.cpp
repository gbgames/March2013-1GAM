#include "SpriteImage.h"

#include "DrawSpriteCommand.h"
#include "IViewPort.h"

SpriteImage::SpriteImage() : 
	m_hotSpot(0, 0),
	m_imageName(""),
	m_imageRect(0, 0, 0, 0)
{
}

SpriteImage::SpriteImage(std::string imageName, Rectangle srcRect, Point hotSpot) : 
	m_hotSpot(hotSpot.X(), hotSpot.Y()),
	m_imageName(imageName),
	m_imageRect(srcRect)
{
}

SpriteImage::~SpriteImage()
{
}

SpriteImage & SpriteImage::operator=(const SpriteImage & rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	m_hotSpot = rhs.hotSpot();
	m_imageName = rhs.spriteImage();
	m_imageRect = rhs.spriteImageRect();

	return *this;
}

Point SpriteImage::hotSpot() const
{
	return m_hotSpot;
}

std::string SpriteImage::spriteImage() const
{
	return m_imageName;
}

Rectangle SpriteImage::spriteImageRect() const
{
	return m_imageRect;
}

void SpriteImage::setHotSpot(Point hotSpot)
{
	if (hotSpot.X() >= 0.0f && hotSpot.X() <= 1.0f &&
		hotSpot.Y() >= 0.0f && hotSpot.Y() <= 1.0f)
	{
		m_hotSpot = hotSpot;
	}
}

void SpriteImage::setImage(std::string imageName, Rectangle srcRect)
{
	m_imageName = imageName;
	m_imageRect = srcRect;
}

IRenderCommand * SpriteImage::draw(Point position, IViewPort * viewPort)
{
	Point scale(1, 1);
	Point offset(0, 0);
	std::string target("");

	if (0 != viewPort)
	{
		scale = viewPort->getScale();
		offset = viewPort->getOffset();
		target = viewPort->getTargetSurface();
	}

	Point scaledPosition = Point(position.X() * scale.X(), position.Y() * scale.Y());
	Point offsetPosition = Point(scaledPosition.X() + offset.X(), scaledPosition.Y() + offset.Y(), position.Z());

	Point renderPosition(offsetPosition.X() - m_imageRect.width() * m_hotSpot.X(), offsetPosition.Y() - m_imageRect.height() * m_hotSpot.Y(), position.Z());
	
	DrawSpriteCommand * cmd = new DrawSpriteCommand();
	cmd->setDrawPortion(m_imageName, m_imageRect, target, offsetPosition, m_hotSpot);

	return cmd;
}
