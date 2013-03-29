#ifndef GB_DRAWRECTANGLECOMMAND_H
#define GB_DRAWRECTANGLECOMMAND_H

#include "IRenderCommand.h"
#include "Rectangle.h"
#include "RenderCommandSet.h"

#include <string>

class IHardwareLayer;

class DrawRectangleCommand : public IRenderCommand
{
	public:
		DrawRectangleCommand(); 
		virtual ~DrawRectangleCommand();

	public:
		virtual void draw(IHardwareLayer & layer); 	
		virtual void add(IRenderCommand * cmd);

		void setRectangle(Point position, Rectangle dimensions, int red, int green, int blue, std::string targetCanvas = "");

	public:
		RenderCommandSet renderSet;
		Rectangle m_dimensions;
		std::string m_dstSpriteName;
		int m_red;
		int m_green;
		int m_blue;
};

#endif
