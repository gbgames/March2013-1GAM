#ifndef GB_ICOMPONENT_H
#define GB_ICOMPONENT_H

#include <string>

class IComponent
{
	public:
		virtual ~IComponent() {}

	public:
		virtual std::string name() const = 0;

};

#endif
