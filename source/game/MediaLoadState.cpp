#include "MediaLoadState.h"

#include "IGame.h"
#include "IHardwareLayer.h"
#include "ISpriteConfig.h"
#include "IStateManager.h"
#include "SpriteImage.h"

#include <sstream>
#include <vector>

MediaLoadState::MediaLoadState(IGame & game, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager) : 
	m_game(game),
	m_spriteConfig(spriteConfig),
	m_spriteManager(spriteManager),
	m_splashScreenSprite(0)
{
}

MediaLoadState::~MediaLoadState()
{
}

void MediaLoadState::onEnter()
{
	//TODO: Ideally should load assets individually and
	// should display a progress bar to player.
	loadFonts();
	loadSounds();

	configureSpriteConfig();
	m_spriteConfig.initialize(m_game.hardwareLayer());
	createSpriteImages();

	m_splashScreenSprite = m_spriteManager.getSprite("SplashScreenSprite");
}

void MediaLoadState::update(int timeElapsed)
{
	m_game.stateManager().clearTo("MainMenu");
}

IRenderCommand * MediaLoadState::draw()
{
	return m_splashScreenSprite->draw(Point(0, 0)); 
}

void MediaLoadState::onExit()
{
	//TODO: Get rid of the Splash Screen sprite. It's not needed anymore.
}

void MediaLoadState::onSuspend()
{
}

void MediaLoadState::onUnsuspend()
{
}

void MediaLoadState::loadFonts()
{
	m_game.hardwareLayer().loadFont("BasicFont", "resources/Fonts/Vera.ttf", 20);
	m_game.hardwareLayer().loadFont("LargeFont", "resources/Fonts/Vera.ttf", 42);
	m_game.hardwareLayer().loadFont("MeterFont", "resources/Fonts/Vera.ttf", 14);
	m_game.hardwareLayer().loadFont("DebugFont", "resources/Fonts/Vera.ttf", 12);
}

void MediaLoadState::loadSounds()
{

	//m_game.hardwareLayer().loadMusic("GameTheme1", "resources/GameTheme1.ogg");
	//m_game.hardwareLayer().loadSound("TrailProduced-Slime", "resources/Trail-Slime1.wav", PLANET_SPRITE_SIZE);

	m_game.hardwareLayer().setMusicVolume(32);
}

void MediaLoadState::configureSpriteConfig()
{
	m_spriteConfig.addSprite("MouseCursor-BasicImage", "resources/cursor-default.png");
	m_spriteConfig.addSprite("MouseCursor-HotImage", "resources/cursor-hand.png");
	m_spriteConfig.addSprite("MouseCursor-ActiveImage", "resources/cursor-hand-down.png");

	m_spriteConfig.addSprite("TitleScreenImage", "resources/TitleScreen.png");
	//m_spriteConfig.addSprite("QuitScreenImage", "resources/QuitScreen.png");
	//m_spriteConfig.addSprite("CreditsScreenImage", "resources/CreditsScreen.png");
	//m_spriteConfig.addSprite("PauseScreenImage", "resources/PauseScreen.png");
	m_spriteConfig.addSprite("ExitScreenImage", "resources/ExitScreen.png");
	m_spriteConfig.addSprite("MenuScreenBackgroundImage", "resources/MenuScreenBackground.png");

	m_spriteConfig.addSprite("BasicBlockImage", "resources/BasicBlock.png");
	m_spriteConfig.addSprite("SemiLitBlockImage", "resources/SemiLitBlock.png");
	m_spriteConfig.addSprite("LitBlockImage", "resources/LitBlock.png");

	m_spriteConfig.addSprite("PlayerShip_UPImage", "resources/PlayerShip_UP.png");
	m_spriteConfig.addSprite("PlayerShip_DOWNImage", "resources/PlayerShip_DOWN.png");
	m_spriteConfig.addSprite("PlayerShip_LEFTImage", "resources/PlayerShip_LEFT.png");
	m_spriteConfig.addSprite("PlayerShip_RIGHTImage", "resources/PlayerShip_RIGHT.png");

	m_spriteConfig.addSprite("MetalPlanetImage", "resources/MetalPlanet.png");
	m_spriteConfig.addSprite("HomeworldPlanetImage", "resources/HomeworldClassPlanet3.png");
	m_spriteConfig.addSprite("RedPlanetImage", "resources/RedPlanet.png");
	m_spriteConfig.addSprite("GasPlanetImage", "resources/GasPlanet.png");
	m_spriteConfig.addSprite("SwirlyGasPlanet1Image", "resources/SwirlyGasPlanet.png");
	m_spriteConfig.addSprite("SwirlyGasPlanet2Image", "resources/SwirlyGasPlanet2.png");
	m_spriteConfig.addSprite("SwirlyGasPlanet3Image", "resources/SwirlyGasPlanet3.png");

	m_spriteConfig.addSprite("SmallMetalPlanetImage", "resources/MetalPlanet-SMALL.png");
	m_spriteConfig.addSprite("SmallHomeworldPlanetImage", "resources/HomeworldClassPlanet3-SMALL.png");
	m_spriteConfig.addSprite("SmallRedPlanetImage", "resources/RedPlanet-SMALL.png");
	m_spriteConfig.addSprite("SmallGasPlanetImage", "resources/GasPlanet-SMALL.png");
	m_spriteConfig.addSprite("SmallSwirlyGasPlanet1Image", "resources/SwirlyGasPlanet-SMALL.png");
	m_spriteConfig.addSprite("SmallSwirlyGasPlanet2Image", "resources/SwirlyGasPlanet2-SMALL.png");
	m_spriteConfig.addSprite("SmallSwirlyGasPlanet3Image", "resources/SwirlyGasPlanet3-SMALL.png");

	m_spriteConfig.addSprite("SmallAnotherSectorImage", "resources/AnotherSector-SMALL.png");

	m_spriteConfig.addSprite("YellowStarImage", "resources/YellowStar.png");

	m_spriteConfig.addSprite("AnotherSectorImage", "resources/AnotherSector.png");
	
	m_spriteConfig.addSprite("MenuOption-AvailableImage", "resources/MenuOption-Available.png");
	m_spriteConfig.addSprite("MenuOption-HotImage", "resources/MenuOption-Hot.png");
	m_spriteConfig.addSprite("MenuOption-ActiveImage", "resources/MenuOption-Active.png");
	m_spriteConfig.addSprite("CancelButton-AvailableImage", "resources/CancelButton-Available.png");
	m_spriteConfig.addSprite("CancelButton-HotImage", "resources/CancelButton-Hot.png");
	m_spriteConfig.addSprite("CancelButton-ActiveImage", "resources/CancelButton-Active.png");

	m_spriteConfig.addSprite("SuperNovaImage", "resources/SuperNova.png");
}


void MediaLoadState::createSpriteImages()
{
	m_spriteManager.createSpriteImage("MouseCursor-BasicSprite", "MouseCursor-BasicImage", Rectangle(0, 0, 25, 29), Point(7.0f/25, 4.0f/29)); 
	m_spriteManager.createSpriteImage("MouseCursor-HotSprite", "MouseCursor-HotImage", Rectangle(0, 0, 25, 29), Point(9.0f/25, 3.0f/29)); 
	m_spriteManager.createSpriteImage("MouseCursor-ActiveSprite", "MouseCursor-ActiveImage", Rectangle(0, 0, 25, 29), Point(9.0f/25, 3.0f/29)); 

	m_spriteManager.createSpriteImage("BasicBlockSprite", "BasicBlockImage", Rectangle(0, 0, 64, 64), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SemiLitBlockSprite", "SemiLitBasicBlockImage", Rectangle(0, 0, 64, 64), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("LitBlockSprite", "LitBasicBlockImage", Rectangle(0, 0, 64, 64), Point(.5, .5)); 

	m_spriteManager.createSpriteImage("PlayerShip_UPSprite", "PlayerShip_UPImage", Rectangle(0, 0, 32, 32), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("PlayerShip_DOWNSprite", "PlayerShip_DOWNImage", Rectangle(0, 0, 32, 32), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("PlayerShip_LEFTSprite", "PlayerShip_LEFTImage", Rectangle(0, 0, 32, 32), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("PlayerShip_RIGHTSprite", "PlayerShip_RIGHTImage", Rectangle(0, 0, 32, 32), Point(.5, .5)); 

	const int PLANET_SPRITE_SIZE = 64;
	m_spriteManager.createSpriteImage("MetalPlanetSprite", "MetalPlanetImage", Rectangle(0, 0, PLANET_SPRITE_SIZE, PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("HomeworldPlanetSprite", "HomeworldPlanetImage", Rectangle(0, 0, PLANET_SPRITE_SIZE, PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("RedPlanetSprite", "RedPlanetImage", Rectangle(0, 0, PLANET_SPRITE_SIZE, PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("GasPlanetSprite", "GasPlanetImage", Rectangle(0, 0, PLANET_SPRITE_SIZE, PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SwirlyGasPlanet1Sprite", "SwirlyGasPlanet1Image", Rectangle(0, 0, PLANET_SPRITE_SIZE, PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SwirlyGasPlanet2Sprite", "SwirlyGasPlanet2Image", Rectangle(0, 0, PLANET_SPRITE_SIZE, PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SwirlyGasPlanet3Sprite", "SwirlyGasPlanet3Image", Rectangle(0, 0, PLANET_SPRITE_SIZE, PLANET_SPRITE_SIZE), Point(.5, .5)); 

	m_spriteManager.createSpriteImage("YellowStarSprite", "YellowStarImage", Rectangle(0, 0, PLANET_SPRITE_SIZE, PLANET_SPRITE_SIZE), Point(.5, .5)); 
	
	m_spriteManager.createSpriteImage("AnotherSectorSprite", "AnotherSectorImage", Rectangle(0, 0, PLANET_SPRITE_SIZE, PLANET_SPRITE_SIZE), Point(.5, .5)); 

	const int SMALL_PLANET_SPRITE_SIZE = 32;
	m_spriteManager.createSpriteImage("SmallMetalPlanetSprite", "SmallMetalPlanetImage", Rectangle(0, 0, SMALL_PLANET_SPRITE_SIZE, SMALL_PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SmallHomeworldPlanetSprite", "SmallHomeworldPlanetImage", Rectangle(0, 0, SMALL_PLANET_SPRITE_SIZE, SMALL_PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SmallRedPlanetSprite", "SmallRedPlanetImage", Rectangle(0, 0, SMALL_PLANET_SPRITE_SIZE, SMALL_PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SmallGasPlanetSprite", "SmallGasPlanetImage", Rectangle(0, 0, SMALL_PLANET_SPRITE_SIZE, SMALL_PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SmallSwirlyGasPlanet1Sprite", "SmallSwirlyGasPlanet1Image", Rectangle(0, 0, SMALL_PLANET_SPRITE_SIZE, SMALL_PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SmallSwirlyGasPlanet2Sprite", "SmallSwirlyGasPlanet2Image", Rectangle(0, 0, SMALL_PLANET_SPRITE_SIZE, SMALL_PLANET_SPRITE_SIZE), Point(.5, .5)); 
	m_spriteManager.createSpriteImage("SmallSwirlyGasPlanet3Sprite", "SmallSwirlyGasPlanet3Image", Rectangle(0, 0, SMALL_PLANET_SPRITE_SIZE, SMALL_PLANET_SPRITE_SIZE), Point(.5, .5)); 

	
	m_spriteManager.createSpriteImage("SmallAnotherSectorSprite", "SmallAnotherSectorImage", Rectangle(0, 0, SMALL_PLANET_SPRITE_SIZE, SMALL_PLANET_SPRITE_SIZE), Point(.5, .5)); 

	m_spriteManager.createSpriteImage("SuperNovaSprite", "SuperNovaImage", Rectangle(0, 0, 256, 256), Point(.5, .5)); 
}
