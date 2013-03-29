#include "UnitTest++.h"

#include "CommandIDs.h"
#include "Option.h"
#include "MenuModel.h"

using namespace GBLib;
using namespace Command;

TEST(MenuModelInitializedWithOptionsAndCommands)
{
	Options options;
	options.push_back(Option(1));
	options.push_back(Option(5));
	options.push_back(Option(3));

	CommandIDs commands;
	commands.push_back(QUIT_APPLICATION);
	commands.push_back(PAUSE_GAME);
	commands.push_back(NO_COMMAND);

	MenuModel model(options, commands);

	CHECK_EQUAL(options.size(), model.numOptions());

	for(unsigned int i = 0; i < options.size(); ++i)
	{
		CHECK_EQUAL(commands.at(i), model.chooseOption(options.at(i)));
	}
}
