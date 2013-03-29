#ifndef GB_ENTITY_H
#define GB_ENTITY_H

#include "Direction.h"
#include "Point.h"

struct Entity
{
	GBLib::Direction orientation;
	Point position;
	bool removeMe;
	
	Entity(GBLib::Direction direction, Point position_) :
		orientation(direction),
		position(position_),
		removeMe(false)
	{
	}

	virtual ~Entity() {}
};

#endif
