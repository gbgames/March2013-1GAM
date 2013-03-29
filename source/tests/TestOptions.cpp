#include "UnitTest++.h"

#include "Option.h"

using namespace GBLib;

TEST(OptionCanBeConfiguredWithID)
{
	OptionID optionID(0);
	Option option(optionID);

	CHECK_EQUAL(optionID, option.ID());
}

TEST(MultipleOptionsCanBeGrouped)
{
	Options options;
	options.push_back(Option(3));
	options.push_back(Option(1));
	options.push_back(Option(5));

	CHECK_EQUAL(3, options.at(0).ID());
	CHECK_EQUAL(1, options.at(1).ID());
	CHECK_EQUAL(5, options.at(2).ID());
}
