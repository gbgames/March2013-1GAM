#ifndef GB_OPTION_H
#define GB_OPTION_H

#include <vector>

namespace GBLib
{

typedef int OptionID;

class Option
{
	public:
		Option(OptionID optionID);

	public:
		OptionID ID() const;

		enum Options
		{
			NO_OPTION = -1
		};

	private:
		OptionID m_optionID;

	private:
		Option();
};

typedef std::vector<Option> Options;

inline bool operator==(const Option & lhs, const Option & rhs)
{
	return lhs.ID() == rhs.ID();
}

inline bool operator<(const Option & lhs, const Option & rhs)
{
	return lhs.ID() < rhs.ID();
}

}

#endif
