#ifndef GB_PARAMETERS_H
#define GB_PARAMETERS_H

#include <string>
#include <vector>

struct Parameter
{
	std::string stringData;
	union
	{
		bool boolData;
		float floatData;
		int intData;
	};

	bool operator==(const Parameter& other) const;
};

typedef std::vector<Parameter> Parameters;

#endif
