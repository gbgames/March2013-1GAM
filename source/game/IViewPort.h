#ifndef GB_IVIEWPORT_H
#define GB_IVIEWPORT_H

#include "Point.h"
#include "Rectangle.h"

#include <string>

class IViewPort
{
	public:
		virtual ~IViewPort() {}

	public:
		virtual Point getScale() = 0;
		virtual Point getOffset() = 0;
		virtual std::string getTargetSurface() = 0;
		virtual Rectangle screenDimensions() = 0;
		virtual Rectangle sourceDimensions() = 0;

};

#endif
