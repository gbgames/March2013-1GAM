#ifndef GB_DRAWSPRITECOMMAND_H
#define GB_DRAWSPRITECOMMAND_H

#include "IRenderCommand.h"
#include "Rectangle.h"
#include "RenderCommandSet.h"

#include <string>

class IHardwareLayer;

class DrawSpriteCommand : public IRenderCommand
{
	public:
		DrawSpriteCommand(); 
		virtual ~DrawSpriteCommand();

	public:
		virtual void draw(IHardwareLayer & layer); 	
		virtual void add(IRenderCommand * cmd);

		void setDrawComplete(std::string spriteName, int dstX, int dstY, int dstZ = 0, std::string dstSurface = "");
		void setDrawComplete(std::string spriteName, Point dstPosition_, std::string dstSurface = "");
		void setDrawPortion(std::string spriteName, int srcX, int srcY, int srcWidth, int srcHeight, std::string dstSurface, int destX, int destY, int destZ = 0, Point hotSpot = Point(0, 0));
		void setDrawPortion(std::string spriteName, Rectangle dimensions, std::string dstSurface, Point dstPosition_, Point hotSpot = Point(0, 0));

	public:
		std::string srcSpriteName;
		Rectangle srcDimensions;
		std::string dstSpriteName;
		RenderCommandSet renderSet;
		Point hotSpot;
};

#endif
