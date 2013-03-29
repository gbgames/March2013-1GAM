#include "DrawSpriteCommand.h"

#include "IHardwareLayer.h"

//#include <iostream>

DrawSpriteCommand::DrawSpriteCommand() : 
	IRenderCommand(0, 0),
	srcSpriteName(""),
	srcDimensions(0, 0, 0, 0),
	dstSpriteName(""),
	hotSpot(0, 0, 0)

{
}

DrawSpriteCommand::~DrawSpriteCommand()
{
}

void DrawSpriteCommand::draw(IHardwareLayer & layer)
{
	//std::cout << "SPRITE ----- " << srcSpriteName << "  ---- @ (" << 
	//dstPosition.X() << ", " << dstPosition.Y() << ", " << dstPosition.Z() << ")" << std::endl;
	if (srcDimensions.width() == 0 && srcDimensions.height() == 0)
	{
		layer.renderSprite(srcSpriteName, dstPosition.intX(), dstPosition.intY(), dstSpriteName);
	}
	else
	{
		int width(srcDimensions.width());
		int height(srcDimensions.height());
		Point renderPosition(dstPosition.X() - width * hotSpot.X(), dstPosition.Y() - height* hotSpot.Y());
		layer.renderSprite(srcSpriteName, srcDimensions.position().intX(), srcDimensions.position().intY(), width, height, dstSpriteName, renderPosition.intX(), renderPosition.intY());
	}

	if (!renderSet.empty())
	{
		//std::cout << ">>>>Drawing sub commandset. " << std::endl;
	}
	renderSet.draw(layer);
	if (!renderSet.empty())
	{
		//std::cout << "<<<Finished sub commandset. " << std::endl;
	}
}

void DrawSpriteCommand::add(IRenderCommand * cmd)
{
	renderSet.add(cmd);
}

void DrawSpriteCommand::setDrawComplete(std::string spriteName, int dstX, int dstY, int dstZ, std::string dstSurface)
{
	srcSpriteName = spriteName;
	srcDimensions.setRectangle(0, 0, 0, 0);
	dstPosition.setPosition(dstX, dstY, dstZ);
	dstSpriteName = dstSurface;
}

void DrawSpriteCommand::setDrawComplete(std::string spriteName, Point dstPosition_, std::string dstSurface)
{
	setDrawComplete(spriteName, dstPosition_.X(), dstPosition_.Y(), dstPosition_.Z(), dstSurface);
}

void DrawSpriteCommand::setDrawPortion(std::string spriteName, int srcX, int srcY, int srcWidth, int srcHeight, std::string dstSurface, int destX, int destY, int destZ, Point hotSpot_)
{
	srcSpriteName = spriteName;
	srcDimensions.setRectangle(srcX, srcY, srcWidth, srcHeight);
	dstPosition = Point(destX, destY, destZ);
	dstSpriteName = dstSurface;
	hotSpot = hotSpot_;
}

void DrawSpriteCommand::setDrawPortion(std::string spriteName, Rectangle dimensions,  std::string dstSurface, Point dstPosition_, Point hotSpot)
{
	setDrawPortion(spriteName, dimensions.position().X(), dimensions.position().Y(), dimensions.width(), dimensions.height(), dstSurface, dstPosition_.X(), dstPosition_.Y(), dstPosition_.Z(), hotSpot);
}

