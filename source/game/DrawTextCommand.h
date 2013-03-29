#ifndef GB_DRAWTEXTCOMMAND_H
#define GB_DRAWTEXTCOMMAND_H

#include "IRenderCommand.h"
#include "RenderCommandSet.h"

#include <string>

class IHardwareLayer;

class DrawTextCommand : public IRenderCommand
{
	public:
		DrawTextCommand(Point position, Point hotSpot, std::string fontName, std::string textToRender, int red, int green, int blue, int alpha = 0, std::string targetCanvas = ""); 
		virtual ~DrawTextCommand();

	public:
		virtual void draw(IHardwareLayer & layer); 	
		virtual void add(IRenderCommand * cmd);

		void setHotSpot(Point hotSpot);

	public:
		RenderCommandSet renderSet;
		std::string m_fontName;
		std::string m_textToRender;
		int m_red;
		int m_green;
		int m_blue;
		int m_alpha;
		Point m_hotSpot;
		std::string m_dstSpriteName;
};

#endif
