#include "OneGAMGame.h"
#include "CommandFactory.h"
#include "ExitScreenState.h"
#include "KeyboardState.h"
#include "InGameState.h"
#include "MainMenuState.h"
#include "MediaLoadState.h"
#include "ObjectFactory.h"
#include "RealSDLInstanceDelegator.h"
#include "SDLHardwareLayer.h"
#include "SDLLayerFactory.h"
#include "SpriteConfig.h"
#include "SpriteManager.h"
#include "StartupState.h"
#include "StateManager.h"
#include "VersionInfo.h"
#include <iostream>

void initializeStateManager(IGame & game, StateManager & stateManager, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager, IVersionInfo & versionInfo);

int main(int argc, char** argv)
{
	RealInstanceDelegator sdlDelegator;
	SDLLayerFactory::replaceInstance(&sdlDelegator);
	KeyboardState keyboardState(sdlDelegator.getNumKeys());
	SDLHardwareLayer hardwareLayer(keyboardState);

	SpriteConfig spriteConfig;
	SpriteManager spriteManager;
	StateManager stateManager;

	ObjectFactory objectFactory;
	CommandFactory commandFactory(objectFactory);
	OneGAMGame game(hardwareLayer, stateManager, commandFactory);
	VersionInfo versionInfo;
	initializeStateManager(game, stateManager, spriteConfig, spriteManager, versionInfo);

	game.initialize();
	unsigned int TIME_STEP = 16;
	game.runWithFixedTimeStep(TIME_STEP);

	return 0;
}

void initializeStateManager(IGame & game, StateManager & stateManager, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager, IVersionInfo & versionInfo)
{
	stateManager.registerState("Startup", new StartupState(game, spriteConfig, spriteManager));
	stateManager.registerState("MediaLoad", new MediaLoadState(game,spriteConfig, spriteManager)); 
	stateManager.registerState("MainMenu", new MainMenuState(game,spriteConfig, spriteManager)); 
	stateManager.registerState("InGame", new InGameState(game, spriteConfig, spriteManager));
	stateManager.registerState("ExitScreen", new ExitScreenState(game, spriteManager, versionInfo));
}
