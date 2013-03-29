#include "Option.h"

using namespace GBLib;

Option::Option(OptionID optionID) : m_optionID(optionID)
{
}

OptionID Option::ID() const
{
	return m_optionID;
}
