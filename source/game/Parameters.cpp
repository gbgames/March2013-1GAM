#include "Parameters.h"

bool Parameter::operator==(const Parameter& other) const
{
	return (other.stringData == this->stringData && other.intData == this->intData);
}
