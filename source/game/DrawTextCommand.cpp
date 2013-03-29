#include "DrawTextCommand.h"

#include "IHardwareLayer.h"

DrawTextCommand::DrawTextCommand(Point position, Point hotSpot, std::string fontName, std::string textToRender, int red, int green, int blue, int alpha, std::string targetCanvas) :
	IRenderCommand(position.X(), position.Y(), position.Z()),
	m_fontName(fontName), 
	m_textToRender(textToRender), 
	m_red(red),
	m_green(green),
	m_blue(blue),
	m_alpha(alpha),
	m_hotSpot(hotSpot),
	m_dstSpriteName(targetCanvas)
{
}

DrawTextCommand::~DrawTextCommand()
{
}

void DrawTextCommand::draw(IHardwareLayer & layer)
{
	Rectangle dimensions = layer.getTextSize(m_fontName, m_textToRender);
	Point finalPosition(dstPosition);
	finalPosition.moveBy(-(dimensions.width() * m_hotSpot.X()), -(dimensions.height() * m_hotSpot.Y()));

	layer.renderTextToSurface(m_fontName, m_textToRender, m_red, m_green, m_blue, m_alpha, finalPosition.X(), finalPosition.Y(), m_dstSpriteName);
}

void DrawTextCommand::add(IRenderCommand * cmd)
{
}

void DrawTextCommand::setHotSpot(Point hotSpot)
{
	m_hotSpot = hotSpot;
}
