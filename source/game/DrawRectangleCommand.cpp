#include "DrawRectangleCommand.h"

#include "IHardwareLayer.h"

DrawRectangleCommand::DrawRectangleCommand() :
	IRenderCommand(0, 0),
	m_dimensions(0, 0, 0, 0),
	m_dstSpriteName(""),
	m_red(0),
	m_green(0),
	m_blue(0)

{
}

DrawRectangleCommand::~DrawRectangleCommand()
{
}

void DrawRectangleCommand::draw(IHardwareLayer & layer)
{
	layer.renderRectangle(dstPosition.intX(), dstPosition.intY(), m_dimensions.width(), m_dimensions.height(), m_red, m_green, m_blue);
	renderSet.draw(layer);
}

void DrawRectangleCommand::add(IRenderCommand * cmd)
{
	renderSet.add(cmd);
}

void DrawRectangleCommand::setRectangle(Point position, Rectangle dimensions, int red, int green, int blue, std::string targetCanvas) 
{
	dstPosition = position;
	m_dimensions = dimensions;
	m_red = red;
	m_green = green;
	m_blue = blue;
	m_dstSpriteName = targetCanvas;
}
