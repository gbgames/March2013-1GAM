#include "InGameState.h"

#include "Color.h"
#include "DrawRectangleCommand.h"
#include "DrawSpriteCommand.h"
#include "Entity.h"
#include "IGame.h"
#include "InputState.h"
#include "ISpriteManager.h"
#include "IStateManager.h"
#include "MenuModel.h"
#include "MenuView.h"
#include "MouseRenderingSystem.h"
#include "SpriteImage.h"
#include "ViewConfiguration.h"
#include "TextLabel.h"
#include "ZOrder.h"
#include <cmath>
#include <cstdlib>
#include <map>
#include <sstream>
#include <iostream>

using namespace GBLib;

namespace PLANET_TYPE
{

enum PlanetTypes
{
	METAL,
	HOMEWORLD,
	RED,
	GAS,
	SWIRLY1,
	SWIRLY2,
	SWIRLY3,
	MAX_PLANETS
};

}

namespace OTHER_SPACE_TYPE
{

enum OtherSpaceTypes
{
	STAR = PLANET_TYPE::MAX_PLANETS + 1,
	SECTOR_UP,
	SECTOR_DOWN,
	SECTOR_LEFT,
	SECTOR_RIGHT,
	THE_HOMEWORLD,
	MAX_TYPES
};

}

namespace GAME_STATE
{

enum GameStates
{
	PLANET_SELECTION_SCREEN,
	PLANET_SCREEN,
	SHOW_SHIP_MOVEMENT,
	DEFEAT_SCREEN,
	VICTORY_SCREEN
};

}

namespace PLANET_SCREEN_OPTIONS
{
	enum PlanetOptions
	{
		NONE,
		CANCEL,
		GOTO,
		SCAN,
		RESUPPLY,
		RESUPPLY_OFF_HOMEWORLD,
		BUILD_BASE,
		UPGRADE_BASE,
		SECTOR_UP,
		SECTOR_DOWN,
		SECTOR_LEFT,
		SECTOR_RIGHT,
		UPGRADE_FUEL_CAPACITY,
		UPGRADE_SUPPLY_CAPACITY,
		UPGRADE_CREW_CAPACITY,
		SCROUNGE_FOR_SUPPLIES,
		MAX_OPTIONS
	};
}

namespace HUD_OPTIONS
{
	enum HUDOptions
	{
		CONVERT_SUPPLIES_TO_FUEL = 1000
	};
}

struct BaseStatus
{
	int baseLevel;
	double timeLeftToBuild;

	BaseStatus(int level, double timeLeft) :
		baseLevel(level),
		timeLeftToBuild(timeLeft)
	{
	}
	
	BaseStatus() :
		baseLevel(0),
		timeLeftToBuild(0)
	{
	}
};

struct PlanetStatus
{
	int lifeType;
	int scanLevel;
	BaseStatus base;
	double resourceAvailability;
};

namespace PLANET_LIFE_STATUS
{
	enum PlanetLifeStatuses
	{
		UNINHABITABLE,
		NONE,
		BASIC_VEGETATION,
		PRIMITIVE,
		LOW_INTELLIGENCE,
		ADVANCED,
		MAX_TYPES
	};
}

namespace PLANET_SCAN_STATUS
{
	enum PlanetScanStatuses
	{
		NONE,
		BASIC,
		COMPLETE
	};
}

namespace PLANET_BASE_STATUS
{
	enum PlanetBaseStatuses
	{
		BUILDING,
		UPGRADING,
		INVALID,
		NONE,
		BASIC,
		UPGRADED
	};
}

namespace PLANET_RESOURCE_AVAILABILITY
{
	enum PlanetResourceAvailabilities
	{
		NONE,
		LOW,
		MIDRANGE,
		HIGH,
		MAX_AVAILABILITIES
	};
}

struct Star
{
	Point position;
	Color color;

	Star(Point pos, int red, int green, int blue) : position(pos), color(red, green, blue) {}
};

namespace FUEL_CAPACITIES
{
	enum FuelCapacities
	{
		BEGINNING,
		INCREASED,
		MAXIMUM
	};
}

namespace SUPPLY_CAPACITIES
{
	enum SupplyCapacities
	{
		BEGINNING,
		INCREASED,
		MAXIMUM
	};
}

namespace CREW_CAPACITIES
{
	enum CrewCapacities
	{
		BEGINNING,
		INCREASED,
		MAXIMUM
	};
}

struct PlayerShip : public Entity
{
	double m_fuel;
	double m_fuelCapacityLevel;
	double m_supplyCapacityLevel;
	double m_supplies;
	int m_currentPlanetIndex;
	double m_mileage;
	double m_speed;
	double m_suppliesCostPerMonth;
	double m_crewCapacityLevel;
	int m_crew;

	PlayerShip(GBLib::Direction direction, Point position_) :
		Entity(direction, position_),
		m_fuel(-1),
		m_supplies(-1),
		m_currentPlanetIndex(-1),
		m_mileage(199.0 / 100.0), // 199 km per 100 units of fuel
		m_speed(225.0 / 6.0), // 225 km to Mars, 6 months to get there
		m_suppliesCostPerMonth(1)
	{
	}

	void resetFuel(double fuel) { m_fuel = fuel; if (0 > m_fuel) m_fuel = 0.0;}
	void resetSupplies(double supplies) { m_supplies = supplies; if (0 > m_supplies) m_supplies = 0;}
	void resetCrew(int crew) { m_crew = crew; if (0 > m_crew) m_crew = 0;}
};

struct Planet 
{
	Point position;
	int planetType;

	Planet(Point pos, int type) : position(pos), planetType(type) {}
};

void addIncome();

void checkSupplies();

struct Homeworld
{
	int wealth;
	Point position;
	double timeUntilNova;
	double lastMonth;

	void countdown(double delta)
	{
		timeUntilNova -= delta;
		while (lastMonth - timeUntilNova > 1)
		{
			checkSupplies();
			lastMonth -= 1;
			addIncome();
		}
	}
	
};

// GAME PLAY VARIABLES
namespace
{
	std::map<int, std::vector<PlanetStatus> > ExploredSectorToPlanetStatuses;
	std::vector<Star> StarField;
	std::vector<Planet> Planets;
	PlayerShip ThePlayerShip(DIRECTION_UP, Point(400, 300, ZORDER::PLAYER_OBJECT));
	Homeworld TheHomeworld;
	int CurrentSectorX(0);
	int CurrentSectorY(0);
	int TargetPlanet(0);
	const int UNIVERSE_WIDTH(100);
	const int UNIVERSE_HEIGHT(100);
	bool GameOver(false);
	std::string DefeatReason("NONE");

	std::map<Direction, SpriteImage *> OrientationToSprite;
	std::map<int, SpriteImage *> PlanetTypeToSprite;
	std::map<int, SpriteImage *> PlanetTypeToLargeSprite;
	SpriteImage * SuperNovaSprite;

	//DEFEAT SCREEN TEXT
	TextLabel DefeatReasonTextLabel(Point(400, 14, ZORDER::HUD), "BasicFont", 0xff, 0x74, 0, Point(0.5, 0));
	TextLabel DefeatTextLabel(Point(400, 32, ZORDER::HUD), "BasicFont", 0xff, 0x74, 0, Point(0.5, 0));

	//VICTORY SCREEN TEXT
	TextLabel VictoryTextLabel(Point(400, 14, ZORDER::HUD), "BasicFont", 0xff, 0x74, 0, Point(0.5, 0));
	TextLabel VictoryTextLabel2(Point(400, 32, ZORDER::HUD), "BasicFont", 0xff, 0x74, 0, Point(0.5, 0));

	//HUD TEXT
	TextLabel CrewLabel(Point(16, 16, ZORDER::HUD), "MeterFont", 0xff, 0x74, 0, Point(0, 0));
	TextLabel CurrentSectorLabel(Point(16, 32, ZORDER::HUD), "MeterFont", 0xff, 0x74, 0, Point(0, 0));
	TextLabel FuelLabel(Point(164, 16, ZORDER::HUD), "MeterFont", 0xff, 0x74, 0, Point(0, 0));
	TextLabel SuppliesLabel(Point(164, 32, ZORDER::HUD), "MeterFont", 0xff, 0x74, 0, Point(0.0, 0));
	TextLabel HomeworldWealthLabel(Point(800-16, 16, ZORDER::HUD), "MeterFont", 0xff, 0x74, 0, Point(1.0, 0));
	TextLabel NovaCountdownLabel(Point(800-16, 32, ZORDER::HUD), "MeterFont", 0xff, 0x74, 0, Point(1.0, 0));

	//MENU TEXT
	TextLabel PlanetMenuTitle(Point(60, 90, ZORDER::HUD), "BasicFont", 255, 255, 255, Point(0.0, 0));
	TextLabel ScanResultsTitle(Point(60, 200, ZORDER::HUD), "BasicFont", 255, 255, 255, Point(0.0, 0));
	TextLabel ScanResultsResourcesLabel(Point(60, 225, ZORDER::HUD), "BasicFont", 255, 255, 255, Point(0.0, 0));
	TextLabel ScanResultsLifeLabel(Point(60, 250, ZORDER::HUD), "BasicFont", 255, 255, 255, Point(0.0, 0));

	int TimeElapsed(0);
	int CurrentGameState(GAME_STATE::PLANET_SELECTION_SCREEN);

	double ShipFuelBeforeLaunch = ThePlayerShip.m_fuel;
	double ShipSuppliesBeforeLaunch = ThePlayerShip.m_supplies;
	double TimeBeforeLaunch = TheHomeworld.timeUntilNova;
	double TotalShipFuelCost = 0.0;
	double TotalShipSuppliesCost = 0.0;
	double TotalTimeCost = 0.0;

	const double BEGINNING_TIME_UNTIL_NOVA = 1200.0;
	const double WINNING_WEALTH = 20000000;

	const double RESUPPLY_TIME_COST = 3.0;

	const double SCAN_FUEL_COST = 10.0;
	const double SCAN_SUPPLIES_COST = 1.0;
	const double SCAN_TIME_COST = 3.0;

	const double BUILD_BASE_FUEL_COST = 30;
	const double BUILD_BASE_SUPPLIES_COST = 5;
	const double BUILD_BASE_TIME_COST = 6;
	const int BUILD_BASE_CREW_COST = 5;
	const int BUILD_BASE_MINIMUM_SHIP_CREW_REQUIREMENT = BUILD_BASE_CREW_COST + 1;


	std::map<int, std::map<int, std::string> > ScanLevelToLifeStatusToText;
	std::map<int, std::map<int, std::string> > ScanLevelToResourceStatusToText;
	std::map<int, std::string> BaseStatusTexts;
	std::map<int, int> PlanetResourceToIncome;
	std::map<int, double> BaseStatusToBaseIncome;

	const double FUEL_UPGRADE_TIME_COST = 3.0;
	std::map<int, double> FuelUpgradeWealthRequirements;
	std::map<int, double> FuelCapacities;

	const double SUPPLY_UPGRADE_TIME_COST = 3.0;
	std::map<int, double> SupplyUpgradeWealthRequirements;
	std::map<int, double> SupplyCapacities;

	const double CREW_UPGRADE_TIME_COST = 3.0;
	std::map<int, double> CrewUpgradeWealthRequirements;
	std::map<int, double> CrewCapacities;
}

double getIncome(const PlanetStatus & planetStatus);

void addIncome()
{
	for (std::map<int, std::vector<PlanetStatus> >::iterator iter = ExploredSectorToPlanetStatuses.begin(); iter != ExploredSectorToPlanetStatuses.end(); ++iter)
	{
		std::vector<PlanetStatus> & planetStatuses = iter->second;
		for (int targetPlanet = 0; targetPlanet < planetStatuses.size(); ++targetPlanet)
		{
			PlanetStatus & planetStatus = planetStatuses.at(targetPlanet);
			BaseStatus & base = planetStatus.base;

			if (PLANET_BASE_STATUS::BUILDING == base.baseLevel || PLANET_BASE_STATUS::UPGRADING == base.baseLevel)
			{
				base.timeLeftToBuild -= 1;
				if (base.timeLeftToBuild <= 0)
				{
					if (PLANET_BASE_STATUS::BUILDING == base.baseLevel)
					{
						base.baseLevel = PLANET_BASE_STATUS::BASIC;
					}
					else
					{
						base.baseLevel = PLANET_BASE_STATUS::UPGRADED;
					}
				}
			}
			else
			{
				TheHomeworld.wealth += getIncome(planetStatus);
			}
		}
	}
}

double getIncome(const PlanetStatus & planetStatus)
{
	const BaseStatus & base = planetStatus.base;
	double baseIncome = BaseStatusToBaseIncome[base.baseLevel];

	int resourceAvailability = planetStatus.resourceAvailability;

	int scanMultiplier = 0.2;
	if (PLANET_SCAN_STATUS::BASIC == planetStatus.scanLevel)
	{
		scanMultiplier = 1.0;
	}
	else if (PLANET_SCAN_STATUS::COMPLETE == planetStatus.scanLevel)
	{
		scanMultiplier = 3.0;
	}

	return baseIncome * PlanetResourceToIncome[resourceAvailability] * scanMultiplier; 
}

void checkSupplies()
{
	if (0 == ThePlayerShip.m_supplies)
	{
		int chanceOfDeath = rand() % 100;
		if (20 > chanceOfDeath)
		{
			ThePlayerShip.resetCrew(ThePlayerShip.m_crew -  1);
		}
	}
}

void generateSector(int sector, std::vector<Star> & starField, std::vector<Planet> & planets, Homeworld & homeWorld)
{
	srand(sector + 123456);

	starField.clear();
	for (int i = 0; i < 1000; ++i)
	{
		starField.push_back(Star(Point(rand() % 800, rand() % 600, ZORDER::BG), 
			rand()%100 + 155, 
			rand()%100 + 155, 
			rand()%100 + 155));
	}

	const double PI = 3.14159265;
	const int MAX_PLANETS_IN_SECTOR(6);
	const double MAX_X_RADIUS(270.0/MAX_PLANETS_IN_SECTOR);
	const double MAX_Y_RADIUS(100.0/MAX_PLANETS_IN_SECTOR);
	int numPlanets = (0 == sector) ? MAX_PLANETS_IN_SECTOR : rand() % MAX_PLANETS_IN_SECTOR + 1;
	planets.clear();

	double startDegrees = rand() % 180;
	int numArcs = numPlanets;
	double arcDistanceInDegrees = 360.0/numArcs;
	for (int i = 1; i < numPlanets + 1; ++i)
	{
		//TODO:  Spread out planets across orbits based on number of planets.
		int planetType = rand() % PLANET_TYPE::MAX_PLANETS;
		float degrees = startDegrees + i * arcDistanceInDegrees;
		float xPos = std::cos((degrees * PI) / 180.0);
		float yPos = std::sin((degrees * PI) / 180.0);
		planets.push_back(Planet(Point(i * MAX_X_RADIUS * xPos + 400, i * MAX_Y_RADIUS * yPos + 300, ZORDER::OBJECT), planetType));
	}

	planets.push_back(Planet(Point(64, 300, ZORDER::OBJECT), OTHER_SPACE_TYPE::SECTOR_LEFT));
	planets.push_back(Planet(Point(800 - 64, 300, ZORDER::OBJECT), OTHER_SPACE_TYPE::SECTOR_RIGHT));
	planets.push_back(Planet(Point(400, 100, ZORDER::OBJECT), OTHER_SPACE_TYPE::SECTOR_UP));
	planets.push_back(Planet(Point(400, 600 - 64, ZORDER::OBJECT), OTHER_SPACE_TYPE::SECTOR_DOWN));

	if (0 == sector)
	{
		planets.push_back(Planet(homeWorld.position, OTHER_SPACE_TYPE::THE_HOMEWORLD));
	}

	if (ExploredSectorToPlanetStatuses.end() == ExploredSectorToPlanetStatuses.find(sector))
	{
		std::vector<PlanetStatus> planetStatuses;
		for (int i = 0; i < planets.size(); ++i)
		{
			PlanetStatus status;
			status.lifeType = PLANET_LIFE_STATUS::ADVANCED;
			status.scanLevel = PLANET_SCAN_STATUS::COMPLETE;
			BaseStatus base(PLANET_BASE_STATUS::UPGRADED, 0);
			status.base = base; 
			status.resourceAvailability = PLANET_RESOURCE_AVAILABILITY::LOW;
			if (OTHER_SPACE_TYPE::THE_HOMEWORLD != planets.at(i).planetType)
			{
				switch (planets.at(i).planetType)
				{
					case OTHER_SPACE_TYPE::SECTOR_UP:
					case OTHER_SPACE_TYPE::SECTOR_DOWN:
					case OTHER_SPACE_TYPE::SECTOR_LEFT:
					case OTHER_SPACE_TYPE::SECTOR_RIGHT:
						{
							status.lifeType = PLANET_LIFE_STATUS::NONE;
							status.base.baseLevel = PLANET_BASE_STATUS::INVALID; 
							break;
						}

					default:
						{
							status.lifeType = rand() % PLANET_LIFE_STATUS::MAX_TYPES;
							status.resourceAvailability = rand() % PLANET_RESOURCE_AVAILABILITY::MAX_AVAILABILITIES;
							status.scanLevel = PLANET_SCAN_STATUS::NONE;
							status.base.baseLevel = PLANET_BASE_STATUS::NONE; 

							break;
						}
				}
			}
			planetStatuses.push_back(status);
		}
		ExploredSectorToPlanetStatuses[sector] = planetStatuses;
	}

}

int currentSector()
{
	return CurrentSectorX + CurrentSectorY * UNIVERSE_WIDTH;
}

void generateMenuBasedOnCurrentSector(int currentSector, std::vector<Planet> planets, IMenuModel ** menuModel, IViewConfiguration ** viewConfiguration, IRenderingSystem ** menuView)
{
	Options options;
	Command::CommandIDs commands;
	int optionNum = 0;
	for (std::vector<Planet>::iterator iter = planets.begin(); iter != planets.end(); ++iter)
	{
		options.push_back(optionNum++);
		commands.push_back(Command::NO_COMMAND);
	}

	// If we have supplies and aren't already maxed out on fuel.
	if (0.5 <= ThePlayerShip.m_supplies && ThePlayerShip.m_fuel < FuelCapacities[ThePlayerShip.m_fuelCapacityLevel])
	{
		options.push_back(HUD_OPTIONS::CONVERT_SUPPLIES_TO_FUEL);
		commands.push_back(Command::NO_COMMAND);
	}

	delete *menuModel;
	*menuModel = new MenuModel(options, commands);

	delete *viewConfiguration;
	*viewConfiguration = new ViewConfiguration(**menuModel);
	for (int i = 0; i < planets.size(); ++i)
	{
		Point planetPosition(planets.at(i).position);
		(*viewConfiguration)->registerButtonControl(options.at(i), "", "", "", "", Rectangle(planetPosition.X() - 16,  planetPosition.Y() - 16, 32, 32));
	}

	if (0.5 <= ThePlayerShip.m_supplies && ThePlayerShip.m_fuel < FuelCapacities[ThePlayerShip.m_fuelCapacityLevel])
	{
		Rectangle convertButtonLocation(150, 60, 128, 64);
		(*viewConfiguration)->registerButtonControl(HUD_OPTIONS::CONVERT_SUPPLIES_TO_FUEL, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", convertButtonLocation);

		//TODO: DOH! Business logic embedded in view?!
		(*viewConfiguration)->registerText("0.5 SUPPLY =", Point( 150 + 128/2, 60 + 64/3, ZORDER::HUD), "MeterFont", 255, 255, 255, Point (0.5, 0.5));
		(*viewConfiguration)->registerText("5 FUEL", Point( 150 + 128/2, 60 + 64/3*2, ZORDER::HUD), "MeterFont", 255, 255, 255, Point (0.5, 0.5));
	}

	delete *menuView;
	*menuView = new MenuView(**menuModel, **viewConfiguration);
}

void generateMenuBasedOnPlanet(int currentSector, std::vector<Planet> planets, int targetPlanet, int currentPlanet, IMenuModel ** menuModel, IViewConfiguration ** viewConfiguration, IRenderingSystem ** menuView)
{
	Options options;
	Command::CommandIDs commands;
	int optionNum = 0;

	options.push_back(Option(PLANET_SCREEN_OPTIONS::CANCEL));
	commands.push_back(Command::NO_COMMAND);

	if (targetPlanet != currentPlanet)
	{
		Point currentPlanetPosition = planets.at(currentPlanet).position;
		double distance = calculateDistance(currentPlanetPosition, planets.at(targetPlanet).position);

		TotalShipFuelCost = distance / ThePlayerShip.m_mileage;
		TotalShipSuppliesCost = distance / ThePlayerShip.m_speed * ThePlayerShip.m_suppliesCostPerMonth;
		TotalTimeCost = distance / ThePlayerShip.m_speed;

		options.push_back(Option(PLANET_SCREEN_OPTIONS::GOTO));
		commands.push_back(Command::NO_COMMAND);
	}
	else
	{
		int optionType(PLANET_SCREEN_OPTIONS::NONE);
		switch(planets.at(targetPlanet).planetType)
		{
			case OTHER_SPACE_TYPE::SECTOR_UP:
			{
				optionType = PLANET_SCREEN_OPTIONS::SECTOR_UP;
				break;
			}

			case OTHER_SPACE_TYPE::SECTOR_DOWN:
			{
				optionType = PLANET_SCREEN_OPTIONS::SECTOR_DOWN;
				break;
			}

			case OTHER_SPACE_TYPE::SECTOR_LEFT:
			{
				optionType = PLANET_SCREEN_OPTIONS::SECTOR_LEFT;
				break;
			}

			case OTHER_SPACE_TYPE::SECTOR_RIGHT:
			{
				optionType = PLANET_SCREEN_OPTIONS::SECTOR_RIGHT;
				break;
			}

			case OTHER_SPACE_TYPE::THE_HOMEWORLD:
			{
				if (FUEL_CAPACITIES::MAXIMUM > ThePlayerShip.m_fuelCapacityLevel && TheHomeworld.wealth > FuelUpgradeWealthRequirements[ThePlayerShip.m_fuelCapacityLevel])
				{
					options.push_back(Option(PLANET_SCREEN_OPTIONS::UPGRADE_FUEL_CAPACITY));
					commands.push_back(Command::NO_COMMAND);
				}

				if (SUPPLY_CAPACITIES::MAXIMUM > ThePlayerShip.m_supplyCapacityLevel && TheHomeworld.wealth > SupplyUpgradeWealthRequirements[ThePlayerShip.m_supplyCapacityLevel])
				{
					options.push_back(Option(PLANET_SCREEN_OPTIONS::UPGRADE_SUPPLY_CAPACITY));
					commands.push_back(Command::NO_COMMAND);
				}
				
				if (CREW_CAPACITIES::MAXIMUM > ThePlayerShip.m_crewCapacityLevel && TheHomeworld.wealth > CrewUpgradeWealthRequirements[ThePlayerShip.m_crewCapacityLevel])
				{
					options.push_back(Option(PLANET_SCREEN_OPTIONS::UPGRADE_CREW_CAPACITY));
					commands.push_back(Command::NO_COMMAND);
				}

				optionType = PLANET_SCREEN_OPTIONS::RESUPPLY;
				break;
			}

			case PLANET_TYPE::METAL:
			case PLANET_TYPE::HOMEWORLD:
			case PLANET_TYPE::RED:
			case PLANET_TYPE::GAS:
			case PLANET_TYPE::SWIRLY1:
			case PLANET_TYPE::SWIRLY2:
			case PLANET_TYPE::SWIRLY3:
			{
				if (PLANET_SCAN_STATUS::COMPLETE > ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).scanLevel)
				{
					optionType = PLANET_SCREEN_OPTIONS::SCAN;
				}

				if(PLANET_BASE_STATUS::INVALID < ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).base.baseLevel && PLANET_BASE_STATUS::UPGRADED > ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).base.baseLevel)
				{
					if (ThePlayerShip.m_crew >= BUILD_BASE_MINIMUM_SHIP_CREW_REQUIREMENT && ThePlayerShip.m_fuel > BUILD_BASE_FUEL_COST && ThePlayerShip.m_supplies > BUILD_BASE_SUPPLIES_COST)
					{

						if(PLANET_LIFE_STATUS::NONE <= ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).lifeType 
								||
								(PLANET_LIFE_STATUS::UNINHABITABLE == ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).lifeType 
								 && PLANET_SCAN_STATUS::COMPLETE > ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).scanLevel)) // If uninhabitable but we don't know it yet, allow player to try.
						{
							options.push_back(Option(PLANET_SCREEN_OPTIONS::BUILD_BASE));
							commands.push_back(Command::NO_COMMAND);
						}
					}
				}

				if (PLANET_BASE_STATUS::UPGRADED == ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).base.baseLevel)
				{

					options.push_back(Option(PLANET_SCREEN_OPTIONS::RESUPPLY_OFF_HOMEWORLD));
					commands.push_back(Command::NO_COMMAND);
				}

				break;
			}

			default:
			{
				std::cout << "SHOULD NOT GET HERE!" << std::endl;
				exit(1);
				break;
			}

		}
		options.push_back(Option(optionType));
		commands.push_back(Command::NO_COMMAND);
	}

	delete *menuModel;
	*menuModel = new MenuModel(options, commands);

	//NOW Create the view.
	delete *viewConfiguration;
	*viewConfiguration = new ViewConfiguration(**menuModel);

	for (Options::iterator iter = options.begin(); iter != options.end(); ++iter)
	{
		OptionID optionID = iter->ID();
		if (PLANET_SCREEN_OPTIONS::CANCEL == optionID)
		{
			Rectangle cancelButtonLocation(705, 90, 32, 32);
			(*viewConfiguration)->registerButtonControl(optionID, "CancelButton-AvailableImage", "CancelButton-AvailableImage", "CancelButton-HotImage", "CancelButton-ActiveImage", cancelButtonLocation);
		}
		else
		{
			if (targetPlanet == currentPlanet)
			{
				switch(optionID)
				{
					case PLANET_SCREEN_OPTIONS::SECTOR_UP:
					case PLANET_SCREEN_OPTIONS::SECTOR_DOWN:
					case PLANET_SCREEN_OPTIONS::SECTOR_LEFT:
					case PLANET_SCREEN_OPTIONS::SECTOR_RIGHT:
						{
							(*viewConfiguration)->registerButtonControl(optionID, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(750 - 10 - 128, 530 - 64, 128, 64));
							(*viewConfiguration)->registerText("JUMP TO", Point(750 - 10 - 128 + 128 /2, 530 - 64 + 64/2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));
							break;
						}

					case PLANET_SCREEN_OPTIONS::SCAN:
						{
							(*viewConfiguration)->registerButtonControl(optionID, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(50 + 10, 530 - 64 , 128, 64));
							(*viewConfiguration)->registerText("SCAN", Point(50 + 10 + 128/2, 530 - 64 + 64/2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));
							

							break;
						}

					case PLANET_SCREEN_OPTIONS::BUILD_BASE:
						{
							(*viewConfiguration)->registerButtonControl(optionID, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(750 - 10 - 128, 530 - 64 , 128, 64));
							(*viewConfiguration)->registerText("BUILD BASE", Point(750 - 10 - 128 + 128/2, 530 - 64 + 64/2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));
							break;
						}
					
					case PLANET_SCREEN_OPTIONS::RESUPPLY:
						{
							(*viewConfiguration)->registerButtonControl(optionID, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(50 + 10, 530 - 64 , 128, 64));
							(*viewConfiguration)->registerText("RESUPPLY", Point(50 + 10 + 128/2, 530 - 64 + 64/2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));

							std::stringstream resupplyCostText;
							resupplyCostText << "Resupply Time: " << RESUPPLY_TIME_COST << " months";
							(*viewConfiguration)->registerText(resupplyCostText.str(), Point(60, 400, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
							break;
						}

					case PLANET_SCREEN_OPTIONS::RESUPPLY_OFF_HOMEWORLD:
						{
							(*viewConfiguration)->registerButtonControl(optionID, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(750 - 10 - 128, 530 - 64 , 128, 64));
							(*viewConfiguration)->registerText("RESUPPLY", Point(750 - 10 - 128 + 128/2, 530 - 64 + 64/2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));
							break;
						}


					case PLANET_SCREEN_OPTIONS::UPGRADE_FUEL_CAPACITY:
						{
							(*viewConfiguration)->registerButtonControl(optionID, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(750 - 10 - 128, 530 - 64 , 128, 64));
							(*viewConfiguration)->registerText("UPGRADE", Point(750 - 10 - 128 + 128/2, 530 - 64 + 64/3, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));
							(*viewConfiguration)->registerText("FUEL", Point(750 - 10 - 128 + 128/2, 530 - 64 + 64/3*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));

							std::stringstream fuelUpgradeCost;
							fuelUpgradeCost << "Time to upgrade: " << FUEL_UPGRADE_TIME_COST << " months";
							(*viewConfiguration)->registerText(fuelUpgradeCost.str(), Point(750 - 10 - 200 + 128/2, 530 - 64 + 64/3, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (1.0, 0.5));

							std::stringstream fuelUpgradeWealthCost;
							fuelUpgradeWealthCost << "Wealth: " << FuelUpgradeWealthRequirements[ThePlayerShip.m_fuelCapacityLevel];
							(*viewConfiguration)->registerText(fuelUpgradeWealthCost.str(), Point(750 - 10 - 200 + 128/2, 530 - 64 + 64/3*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (1.0, 0.5));

							break;
						}

					case PLANET_SCREEN_OPTIONS::UPGRADE_SUPPLY_CAPACITY:
						{
							(*viewConfiguration)->registerButtonControl(optionID, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(750 - 10 - 128, 530 - 2*64 , 128, 64));
							(*viewConfiguration)->registerText("UPGRADE", Point(750 - 10 - 128 + 128/2, 530 - 2*64 + 64/3, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));
							(*viewConfiguration)->registerText("SUPPLY", Point(750 - 10 - 128 + 128/2, 530 - 2*64 + 64/3*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));

							std::stringstream supplyUpgradeCost;
							supplyUpgradeCost << "Time to upgrade: " << SUPPLY_UPGRADE_TIME_COST << " months";
							(*viewConfiguration)->registerText(supplyUpgradeCost.str(), Point(750 - 10 - 200 + 128/2, 530 - 2*64 + 64/3, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (1.0, 0.5));
							std::stringstream supplyUpgradeWealthCost;
							supplyUpgradeWealthCost << "Wealth: " << SupplyUpgradeWealthRequirements[ThePlayerShip.m_supplyCapacityLevel];
							(*viewConfiguration)->registerText(supplyUpgradeWealthCost.str(), Point(750 - 10 - 200 + 128/2, 530 - 2*64 + 64/3*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (1.0, 0.5));

							break;
						}
					
					case PLANET_SCREEN_OPTIONS::UPGRADE_CREW_CAPACITY:
						{
							(*viewConfiguration)->registerButtonControl(optionID, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(750 - 10 - 128, 530 - 3*64 , 128, 64));
							(*viewConfiguration)->registerText("UPGRADE", Point(750 - 10 - 128 + 128/2, 530 - 3*64 + 64/3, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));
							(*viewConfiguration)->registerText("CREW", Point(750 - 10 - 128 + 128/2, 530 - 3*64 + 64/3*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));

							std::stringstream crewUpgradeCost;
							crewUpgradeCost << "Time to upgrade: " << CREW_UPGRADE_TIME_COST << " months";
							(*viewConfiguration)->registerText(crewUpgradeCost.str(), Point(750 - 10 - 200 + 128/2, 530 - 3*64 + 64/3, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (1.0, 0.5));
							std::stringstream crewUpgradeWealthCost;
							crewUpgradeWealthCost << "Wealth: " << CrewUpgradeWealthRequirements[ThePlayerShip.m_crewCapacityLevel];
							(*viewConfiguration)->registerText(crewUpgradeWealthCost.str(), Point(750 - 10 - 200 + 128/2, 530 - 3*64 + 64/3*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (1.0, 0.5));

							break;
						}
				}
			}
			else
			{
				switch(optionID)
				{
					case PLANET_SCREEN_OPTIONS::GOTO:
						{
							(*viewConfiguration)->registerButtonControl(optionID, "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(50 + 10, 530 - 64, 128, 64));
							(*viewConfiguration)->registerText("GO TO", Point(50 + 10 + 128/2, 530 - 64 + 64/2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));
							break;
						}
				}
			}
		}
	}

	if (targetPlanet != currentPlanet)
	{
		std::stringstream distanceText;
		Point currentPlanetPosition = planets.at(currentPlanet).position;
		double distance = calculateDistance(currentPlanetPosition, planets.at(targetPlanet).position);
		distanceText << "Distance: " << distance  << " million kilometers"; 
		(*viewConfiguration)->registerText(distanceText.str(), Point(60, 115, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

		//DISPLAY TRAVEL COSTS
		{
			int yPos=350;
			(*viewConfiguration)->registerText("TRAVEL COSTS:", Point(60, yPos, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

			std::stringstream travelFuelCostText;
			travelFuelCostText.precision(3);
			travelFuelCostText << "Fuel - " << TotalShipFuelCost;
			(*viewConfiguration)->registerText(travelFuelCostText.str(), Point(60, yPos + 25*1, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

			std::stringstream travelSuppliesCostText;
			travelSuppliesCostText.precision(3);
			travelSuppliesCostText << "Supplies - " << TotalShipSuppliesCost;
			(*viewConfiguration)->registerText(travelSuppliesCostText.str(), Point(60, yPos + 25*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

			std::stringstream travelTimeCostText;
			travelTimeCostText.precision(3);
			travelTimeCostText << "Time - " << TotalTimeCost << " Months";
			(*viewConfiguration)->registerText(travelTimeCostText.str(), Point(60, yPos + 25*3, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

		}

		//DISPLAY BASE DATA
		{
			int yPos=250;
			int index = 0;
			BaseStatus & base = ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).base;

			if (PLANET_BASE_STATUS::INVALID != base.baseLevel)
			{
				(*viewConfiguration)->registerText("BASE STATUS", Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
				(*viewConfiguration)->registerText(BaseStatusTexts[base.baseLevel], Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
				if (PLANET_BASE_STATUS::BASIC <= base.baseLevel)
				{
					std::stringstream incomeText;
					incomeText << "Income: " << getIncome(ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet));
					(*viewConfiguration)->registerText(incomeText.str(), Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
				}
			}

			if (PLANET_BASE_STATUS::INVALID > base.baseLevel) //is building or upgrading
			{
				std::stringstream baseTimeCostText;
				baseTimeCostText.precision(3);
				baseTimeCostText << "Time Left - " 
					<< ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).base.timeLeftToBuild
					<< " Months";
				(*viewConfiguration)->registerText(baseTimeCostText.str(), Point(500, yPos + 25*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
			}
		}
	}
	else
	{
		//DISPLAY BASE BUILDING COSTS
		{
			int yPos=250;
			int index = 0;
			BaseStatus & base = ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).base;

			if (PLANET_BASE_STATUS::INVALID != base.baseLevel)
			{
				(*viewConfiguration)->registerText("BASE STATUS", Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
				(*viewConfiguration)->registerText(BaseStatusTexts[base.baseLevel], Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
				if (PLANET_BASE_STATUS::BASIC <= base.baseLevel)
				{
					std::stringstream incomeText;
					incomeText << "Income: " << getIncome(ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet));
					(*viewConfiguration)->registerText(incomeText.str(), Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
				}

				if (PLANET_BASE_STATUS::INVALID < base.baseLevel && PLANET_BASE_STATUS::UPGRADED > base.baseLevel)
				{
					(*viewConfiguration)->registerText("BASE BUILDING COSTS", Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

					std::stringstream baseCrewCostText;
					baseCrewCostText << "Staff - " <<  BUILD_BASE_CREW_COST;
					(*viewConfiguration)->registerText(baseCrewCostText.str(), Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

					std::stringstream baseFuelCostText;
					baseFuelCostText.precision(3);
					baseFuelCostText << "Fuel - " << BUILD_BASE_FUEL_COST;
					(*viewConfiguration)->registerText(baseFuelCostText.str(), Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

					std::stringstream baseSuppliesCostText;
					baseSuppliesCostText.precision(3);
					baseSuppliesCostText << "Supplies - " << BUILD_BASE_SUPPLIES_COST;
					(*viewConfiguration)->registerText(baseSuppliesCostText.str(), Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

					std::stringstream baseTimeCostText;
					baseTimeCostText.precision(3);
					baseTimeCostText << "Time - " << BUILD_BASE_TIME_COST << " Months";
					(*viewConfiguration)->registerText(baseTimeCostText.str(), Point(500, yPos + 25*index++, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
				}
				else if (PLANET_BASE_STATUS::INVALID > base.baseLevel) //is building or upgrading
				{
					std::stringstream baseTimeCostText;
					baseTimeCostText.precision(3);
					baseTimeCostText << "Time Left - " 
						<< ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).base.timeLeftToBuild
						<< " Months";
					(*viewConfiguration)->registerText(baseTimeCostText.str(), Point(500, yPos + 25*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
				}
			}
			
		}

		// DISPLAY SCAN COSTS
		if (PLANET_SCAN_STATUS::COMPLETE > ExploredSectorToPlanetStatuses[currentSector].at(targetPlanet).scanLevel)
		{
			int yPos=350;
			(*viewConfiguration)->registerText("SCAN COSTS:", Point(60, yPos, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

			std::stringstream scanFuelCostText;
			scanFuelCostText.precision(3);
			scanFuelCostText << "Fuel - " << SCAN_FUEL_COST;
			(*viewConfiguration)->registerText(scanFuelCostText.str(), Point(60, yPos + 25*1, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

			std::stringstream scanSuppliesCostText;
			scanSuppliesCostText.precision(3);
			scanSuppliesCostText << "Supplies - " << SCAN_SUPPLIES_COST;
			(*viewConfiguration)->registerText(scanSuppliesCostText.str(), Point(60, yPos + 25*2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));

			std::stringstream scanTimeCostText;
			scanTimeCostText.precision(3);
			scanTimeCostText << "Time - " << SCAN_TIME_COST << " Months";
			(*viewConfiguration)->registerText(scanTimeCostText.str(), Point(60, yPos + 25*3, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.0, 0.0));
		}
	}

	delete *menuView;
	*menuView = new MenuView(**menuModel, **viewConfiguration);
}

void generateMenuBasedOnDefeat(IMenuModel ** menuModel, IViewConfiguration ** viewConfiguration, IRenderingSystem ** menuView)
{
	srand(123456);

	StarField.clear();
	for (int i = 0; i < 1000; ++i)
	{
		StarField.push_back(Star(Point(rand() % 800, rand() % 600, ZORDER::BG), 
			rand()%100 + 155, 
			rand()%100 + 155, 
			rand()%100 + 155));
	}

	Planets.clear();

	Options options;
	Command::CommandIDs commands;
	options.push_back(OptionID(1));
	commands.push_back(Command::NO_COMMAND);

	delete *menuModel;
	*menuModel = new MenuModel(options, commands);

	delete *viewConfiguration;
	*viewConfiguration = new ViewConfiguration(**menuModel);
	(*viewConfiguration)->registerButtonControl(options.back(), "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(750 - 10 - 128, 530 - 64, 128, 64));
	(*viewConfiguration)->registerText("PLAY AGAIN", Point(750 - 10 - 128 + 128 /2, 530 - 64 + 64/2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));

	delete *menuView;
	*menuView = new MenuView(**menuModel, **viewConfiguration);

	if (0 >= ThePlayerShip.m_crew)
	{
		DefeatReason = "Your have lost your crew.";
	}
	else
	{
		DefeatReason = "Your homeworld's sun has gone nova.";
	}
}

void generateMenuBasedOnVictory(IMenuModel ** menuModel, IViewConfiguration ** viewConfiguration, IRenderingSystem ** menuView)
{
	srand(123456);

	StarField.clear();
	for (int i = 0; i < 1000; ++i)
	{
		StarField.push_back(Star(Point(rand() % 800, rand() % 600, ZORDER::BG), 
			rand()%100 + 155, 
			rand()%100 + 155, 
			rand()%100 + 155));
	}

	Planets.clear();

	Options options;
	Command::CommandIDs commands;
	options.push_back(OptionID(1));
	commands.push_back(Command::NO_COMMAND);

	delete *menuModel;
	*menuModel = new MenuModel(options, commands);

	delete *viewConfiguration;
	*viewConfiguration = new ViewConfiguration(**menuModel);
	(*viewConfiguration)->registerButtonControl(options.back(), "MenuOption-AvailableImage", "MenuOption-AvailableImage", "MenuOption-HotImage", "MenuOption-ActiveImage", Rectangle(750 - 10 - 128, 530 - 64, 128, 64));
	(*viewConfiguration)->registerText("PLAY AGAIN", Point(750 - 10 - 128 + 128 /2, 530 - 64 + 64/2, ZORDER::HUD), "BasicFont", 255, 255, 255, Point (0.5, 0.5));

	delete *menuView;
	*menuView = new MenuView(**menuModel, **viewConfiguration);
}

void InGameState::reset()
{
	CurrentSectorX = 0;
	CurrentSectorY = 0;
	TheHomeworld.wealth = 100;
	TheHomeworld.position = Point(400, 300, ZORDER::OBJECT);
	TheHomeworld.timeUntilNova = BEGINNING_TIME_UNTIL_NOVA;
	TheHomeworld.lastMonth = TheHomeworld.timeUntilNova;

	TimeElapsed = 0;
	CurrentGameState = GAME_STATE::PLANET_SELECTION_SCREEN;

	ExploredSectorToPlanetStatuses.clear();
	generateSector(currentSector(), StarField, Planets, TheHomeworld);
	generateMenuBasedOnCurrentSector(currentSector(), Planets, &m_menuModel, &m_viewConfiguration, &m_menuView);

	ThePlayerShip.m_fuelCapacityLevel = FUEL_CAPACITIES::BEGINNING;
	ThePlayerShip.resetFuel(FuelCapacities[ThePlayerShip.m_fuelCapacityLevel]);
	ThePlayerShip.m_supplyCapacityLevel = SUPPLY_CAPACITIES::BEGINNING;
	ThePlayerShip.resetSupplies(SupplyCapacities[ThePlayerShip.m_supplyCapacityLevel]);
	ThePlayerShip.m_crewCapacityLevel = CREW_CAPACITIES::BEGINNING;
	ThePlayerShip.resetCrew(CrewCapacities[ThePlayerShip.m_crewCapacityLevel]);
	ThePlayerShip.m_currentPlanetIndex = Planets.size() - 1;
	ThePlayerShip.position.setPosition(400, 300, ZORDER::OBJECT);
	GameOver = false;
}

InGameState::InGameState(IGame & game, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager) :
	m_game(game),
	m_spriteConfig(spriteConfig),
	m_spriteManager(spriteManager),
	m_stateSuspended(false),
	m_menuModel(0),
	m_viewConfiguration(0),
	m_menuView(0),
	m_mouseRenderer(0)
{
}

InGameState::~InGameState()
{
}

void InGameState::onEnter()
{
	delete m_mouseRenderer;
	m_mouseRenderer = new MouseRenderingSystem(m_spriteManager);
	
	OrientationToSprite.clear();
	OrientationToSprite[DIRECTION_UP] = m_spriteManager.getSprite("PlayerShip_UPSprite");
	OrientationToSprite[DIRECTION_DOWN] = m_spriteManager.getSprite("PlayerShip_DOWNImage");
	OrientationToSprite[DIRECTION_LEFT] = m_spriteManager.getSprite("PlayerShip_LEFTImage");
	OrientationToSprite[DIRECTION_RIGHT] = m_spriteManager.getSprite("PlayerShip_RIGHTImage");

	PlanetTypeToSprite.clear();
	PlanetTypeToSprite[PLANET_TYPE::METAL] = m_spriteManager.getSprite("SmallMetalPlanetSprite");
	PlanetTypeToSprite[PLANET_TYPE::HOMEWORLD] = m_spriteManager.getSprite("SmallHomeworldPlanetSprite");
	PlanetTypeToSprite[PLANET_TYPE::RED] = m_spriteManager.getSprite("SmallRedPlanetSprite");
	PlanetTypeToSprite[PLANET_TYPE::GAS] = m_spriteManager.getSprite("SmallGasPlanetSprite");
	PlanetTypeToSprite[PLANET_TYPE::SWIRLY1] = m_spriteManager.getSprite("SmallSwirlyGasPlanet1Sprite");
	PlanetTypeToSprite[PLANET_TYPE::SWIRLY2] = m_spriteManager.getSprite("SmallSwirlyGasPlanet2Sprite");
	PlanetTypeToSprite[PLANET_TYPE::SWIRLY3] = m_spriteManager.getSprite("SmallSwirlyGasPlanet3Sprite");
	
	PlanetTypeToSprite[OTHER_SPACE_TYPE::STAR] = m_spriteManager.getSprite("YellowStarSprite");
	PlanetTypeToSprite[OTHER_SPACE_TYPE::SECTOR_UP] = m_spriteManager.getSprite("SmallAnotherSectorSprite");
	PlanetTypeToSprite[OTHER_SPACE_TYPE::SECTOR_DOWN] = m_spriteManager.getSprite("SmallAnotherSectorSprite");
	PlanetTypeToSprite[OTHER_SPACE_TYPE::SECTOR_LEFT] = m_spriteManager.getSprite("SmallAnotherSectorSprite");
	PlanetTypeToSprite[OTHER_SPACE_TYPE::SECTOR_RIGHT] = m_spriteManager.getSprite("SmallAnotherSectorSprite");
	PlanetTypeToSprite[OTHER_SPACE_TYPE::THE_HOMEWORLD] = m_spriteManager.getSprite("SmallHomeworldPlanetSprite");

	PlanetTypeToLargeSprite.clear();
	PlanetTypeToLargeSprite[PLANET_TYPE::METAL] = m_spriteManager.getSprite("MetalPlanetSprite");
	PlanetTypeToLargeSprite[PLANET_TYPE::HOMEWORLD] = m_spriteManager.getSprite("HomeworldPlanetSprite");
	PlanetTypeToLargeSprite[PLANET_TYPE::RED] = m_spriteManager.getSprite("RedPlanetSprite");
	PlanetTypeToLargeSprite[PLANET_TYPE::GAS] = m_spriteManager.getSprite("GasPlanetSprite");
	PlanetTypeToLargeSprite[PLANET_TYPE::SWIRLY1] = m_spriteManager.getSprite("SwirlyGasPlanet1Sprite");
	PlanetTypeToLargeSprite[PLANET_TYPE::SWIRLY2] = m_spriteManager.getSprite("SwirlyGasPlanet2Sprite");
	PlanetTypeToLargeSprite[PLANET_TYPE::SWIRLY3] = m_spriteManager.getSprite("SwirlyGasPlanet3Sprite");
	
	PlanetTypeToLargeSprite[OTHER_SPACE_TYPE::STAR] = m_spriteManager.getSprite("YellowStarSprite");
	PlanetTypeToLargeSprite[OTHER_SPACE_TYPE::SECTOR_UP] = m_spriteManager.getSprite("AnotherSectorSprite");
	PlanetTypeToLargeSprite[OTHER_SPACE_TYPE::SECTOR_DOWN] = m_spriteManager.getSprite("AnotherSectorSprite");
	PlanetTypeToLargeSprite[OTHER_SPACE_TYPE::SECTOR_LEFT] = m_spriteManager.getSprite("AnotherSectorSprite");
	PlanetTypeToLargeSprite[OTHER_SPACE_TYPE::SECTOR_RIGHT] = m_spriteManager.getSprite("AnotherSectorSprite");
	PlanetTypeToLargeSprite[OTHER_SPACE_TYPE::THE_HOMEWORLD] = m_spriteManager.getSprite("HomeworldPlanetSprite");

	ScanLevelToLifeStatusToText.clear();
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_LIFE_STATUS::NONE] = "Unknown";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_LIFE_STATUS::UNINHABITABLE] = "Unknown";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_LIFE_STATUS::BASIC_VEGETATION] = "Unknown";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_LIFE_STATUS::PRIMITIVE] = "Unknown";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_LIFE_STATUS::LOW_INTELLIGENCE] = "Unknown";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_LIFE_STATUS::ADVANCED] = "Unknown";

	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_LIFE_STATUS::NONE] = "None";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_LIFE_STATUS::UNINHABITABLE] = "Unknown";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_LIFE_STATUS::BASIC_VEGETATION] = "Some Plants";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_LIFE_STATUS::PRIMITIVE] = "Some Animals & Plants";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_LIFE_STATUS::LOW_INTELLIGENCE] = "Primitive";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_LIFE_STATUS::ADVANCED] = "Advanced";

	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_LIFE_STATUS::NONE] = "None";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_LIFE_STATUS::UNINHABITABLE] = "Uninhabitable";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_LIFE_STATUS::BASIC_VEGETATION] = "Basic Flora";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_LIFE_STATUS::PRIMITIVE] = "Basic Fauna & Flora";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_LIFE_STATUS::LOW_INTELLIGENCE] = "Primitive";
	ScanLevelToLifeStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_LIFE_STATUS::ADVANCED] = "Advanced";

	ScanLevelToResourceStatusToText.clear();
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_RESOURCE_AVAILABILITY::NONE] = "Unknown";
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_RESOURCE_AVAILABILITY::LOW] = "Unknown";
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_RESOURCE_AVAILABILITY::MIDRANGE] = "Unknown";
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::NONE][PLANET_RESOURCE_AVAILABILITY::HIGH] = "Unknown";

	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_RESOURCE_AVAILABILITY::NONE] = "Unknown";
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_RESOURCE_AVAILABILITY::LOW] = "Unknown";
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_RESOURCE_AVAILABILITY::MIDRANGE] = "Some";
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::BASIC][PLANET_RESOURCE_AVAILABILITY::HIGH] = "Some";

	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_RESOURCE_AVAILABILITY::NONE] = "None";
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_RESOURCE_AVAILABILITY::LOW] = "Poor";
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_RESOURCE_AVAILABILITY::MIDRANGE] = "Some";
	ScanLevelToResourceStatusToText[PLANET_SCAN_STATUS::COMPLETE][PLANET_RESOURCE_AVAILABILITY::HIGH] = "Rich";

	BaseStatusTexts.clear();
	BaseStatusTexts[PLANET_BASE_STATUS::BUILDING] = "Under construction.";
	BaseStatusTexts[PLANET_BASE_STATUS::UPGRADING] = "Upgrading.";
	BaseStatusTexts[PLANET_BASE_STATUS::NONE] = "None.";
	BaseStatusTexts[PLANET_BASE_STATUS::BASIC] = "Level 1 Base.";
	BaseStatusTexts[PLANET_BASE_STATUS::UPGRADED] = "Level 2 Base.";

	PlanetResourceToIncome.clear();
	PlanetResourceToIncome[PLANET_RESOURCE_AVAILABILITY::NONE] = 0;
	PlanetResourceToIncome[PLANET_RESOURCE_AVAILABILITY::LOW] = 1;
	PlanetResourceToIncome[PLANET_RESOURCE_AVAILABILITY::MIDRANGE] = 5;
	PlanetResourceToIncome[PLANET_RESOURCE_AVAILABILITY::HIGH] = 10;

	BaseStatusToBaseIncome.clear();
	BaseStatusToBaseIncome[PLANET_BASE_STATUS::NONE] = 0;
	BaseStatusToBaseIncome[PLANET_BASE_STATUS::BASIC] = 1;
	BaseStatusToBaseIncome[PLANET_BASE_STATUS::UPGRADED] = 3;

	FuelUpgradeWealthRequirements[FUEL_CAPACITIES::BEGINNING] = 200;
	FuelUpgradeWealthRequirements[FUEL_CAPACITIES::INCREASED] = 3000;

	FuelCapacities[FUEL_CAPACITIES::BEGINNING] = 500.0; 
	FuelCapacities[FUEL_CAPACITIES::INCREASED] = 1000.0; 
	FuelCapacities[FUEL_CAPACITIES::MAXIMUM] = 5000.0; 

	SupplyUpgradeWealthRequirements[SUPPLY_CAPACITIES::BEGINNING] = 200;
	SupplyUpgradeWealthRequirements[SUPPLY_CAPACITIES::INCREASED] = 3000;

	SupplyCapacities[SUPPLY_CAPACITIES::BEGINNING] = 36;
	SupplyCapacities[SUPPLY_CAPACITIES::INCREASED] = 100;
	SupplyCapacities[SUPPLY_CAPACITIES::MAXIMUM] = 500;

	CrewUpgradeWealthRequirements[CREW_CAPACITIES::BEGINNING] = 200;
	CrewUpgradeWealthRequirements[CREW_CAPACITIES::INCREASED] = 3000;

	CrewCapacities[CREW_CAPACITIES::BEGINNING] = 25;
	CrewCapacities[CREW_CAPACITIES::INCREASED] = 75;
	CrewCapacities[CREW_CAPACITIES::MAXIMUM] = 200;

	SuperNovaSprite = m_spriteManager.getSprite("SuperNovaSprite");

	reset();	
}

void InGameState::update(int timeElapsed)
{
	if (!GameOver)
	{
		if (0 >= ThePlayerShip.m_crew || 0 >= TheHomeworld.timeUntilNova)
		{
			GameOver = true;
			CurrentGameState = GAME_STATE::DEFEAT_SCREEN;
			generateMenuBasedOnDefeat(&m_menuModel, &m_viewConfiguration, &m_menuView);
		}
		else if (WINNING_WEALTH <= TheHomeworld.wealth)
		{
			GameOver = true;
			CurrentGameState = GAME_STATE::VICTORY_SCREEN;
			generateMenuBasedOnVictory(&m_menuModel, &m_viewConfiguration, &m_menuView);
		}

		
	}

	switch(CurrentGameState)
	{
		case GAME_STATE::DEFEAT_SCREEN:
			{
				Option option = m_menuView->update(m_game.getInputState());
				if (Option::NO_OPTION != option.ID())
				{
					reset();
				}
				break;
			}
		case GAME_STATE::VICTORY_SCREEN:
			{
				Option option = m_menuView->update(m_game.getInputState());
				if (Option::NO_OPTION != option.ID())
				{
					reset();
				}
				break;
			}

		case GAME_STATE::PLANET_SELECTION_SCREEN:
			{
				Option option = m_menuView->update(m_game.getInputState());
				if (Option::NO_OPTION != option.ID())
				{
					if (HUD_OPTIONS::CONVERT_SUPPLIES_TO_FUEL == option.ID())
					{
						ThePlayerShip.resetSupplies(ThePlayerShip.m_supplies - 0.5);
						ThePlayerShip.resetFuel(ThePlayerShip.m_fuel + 5);
						if (ThePlayerShip.m_fuel > FuelCapacities[ThePlayerShip.m_fuelCapacityLevel])
						{
							ThePlayerShip.m_fuel = FuelCapacities[ThePlayerShip.m_fuelCapacityLevel];
						}
						generateMenuBasedOnCurrentSector(currentSector(), Planets, &m_menuModel, &m_viewConfiguration, &m_menuView);
					}
					else
					{
						CurrentGameState = GAME_STATE::PLANET_SCREEN;
						TargetPlanet = option.ID();
						generateMenuBasedOnPlanet(currentSector(), Planets, TargetPlanet, ThePlayerShip.m_currentPlanetIndex, &m_menuModel, &m_viewConfiguration, &m_menuView);
					}
				}

				break;
			}

		case GAME_STATE::PLANET_SCREEN:
			{
				Option option = m_menuView->update(m_game.getInputState());
				if (Option::NO_OPTION != option.ID())
				{

					switch(option.ID())
					{
						case PLANET_SCREEN_OPTIONS::GOTO:
							{
								ShipFuelBeforeLaunch = ThePlayerShip.m_fuel;
								ShipSuppliesBeforeLaunch = ThePlayerShip.m_supplies;
								TimeBeforeLaunch = TheHomeworld.timeUntilNova;
								bool shipHasEnoughFuelForTrip = 
									(0 <= ShipFuelBeforeLaunch - TotalShipFuelCost); 

								if (shipHasEnoughFuelForTrip)
								{
									CurrentGameState = GAME_STATE::SHOW_SHIP_MOVEMENT;
									delete m_menuView;
									m_menuView = 0;
									TimeElapsed = 0;
								}

								break;
							}

						case PLANET_SCREEN_OPTIONS::CANCEL:
							{
								CurrentGameState = GAME_STATE::PLANET_SELECTION_SCREEN;
								generateMenuBasedOnCurrentSector(currentSector(), Planets, &m_menuModel, &m_viewConfiguration, &m_menuView);
								break;
							}
						case PLANET_SCREEN_OPTIONS::SECTOR_RIGHT:
							{
								CurrentGameState = GAME_STATE::PLANET_SELECTION_SCREEN;
								++CurrentSectorX; 
								if (CurrentSectorX >= UNIVERSE_WIDTH)
								{
									CurrentSectorX = 0;
								}

								generateSector(currentSector(), StarField, Planets, TheHomeworld);
								generateMenuBasedOnCurrentSector(currentSector(), Planets, &m_menuModel, &m_viewConfiguration, &m_menuView);
								for (int i = 0; i  < Planets.size(); ++i)
								{
									if (OTHER_SPACE_TYPE::SECTOR_LEFT == Planets.at(i).planetType)
									{
										ThePlayerShip.m_currentPlanetIndex = i;
									}
								}
								TargetPlanet = ThePlayerShip.m_currentPlanetIndex;
								Point targetPosition = Planets.at(ThePlayerShip.m_currentPlanetIndex).position;
								ThePlayerShip.position.setPosition(targetPosition.X(), targetPosition.Y(), ZORDER::PLAYER_OBJECT);
								break;
							}
						case PLANET_SCREEN_OPTIONS::SECTOR_LEFT:
							{
								CurrentGameState = GAME_STATE::PLANET_SELECTION_SCREEN;
								--CurrentSectorX;
								if (CurrentSectorX < 0)
								{
									CurrentSectorX = UNIVERSE_WIDTH - 1;
								}

								generateSector(currentSector(), StarField, Planets, TheHomeworld);
								generateMenuBasedOnCurrentSector(currentSector(), Planets, &m_menuModel, &m_viewConfiguration, &m_menuView);
								for (int i = 0; i  < Planets.size(); ++i)
								{
									if (OTHER_SPACE_TYPE::SECTOR_RIGHT == Planets.at(i).planetType)
									{
										ThePlayerShip.m_currentPlanetIndex = i;
									}
								}
								TargetPlanet = ThePlayerShip.m_currentPlanetIndex;
								Point targetPosition = Planets.at(ThePlayerShip.m_currentPlanetIndex).position;
								ThePlayerShip.position.setPosition(targetPosition.X(), targetPosition.Y(), ZORDER::PLAYER_OBJECT);
								break;
							}
						case PLANET_SCREEN_OPTIONS::SECTOR_UP:
							{
								CurrentGameState = GAME_STATE::PLANET_SELECTION_SCREEN;
								--CurrentSectorY;
								if (CurrentSectorY < 0)
								{
									CurrentSectorY = UNIVERSE_HEIGHT - 1;
								}

								generateSector(currentSector(), StarField, Planets, TheHomeworld);
								generateMenuBasedOnCurrentSector(currentSector(), Planets, &m_menuModel, &m_viewConfiguration, &m_menuView);
								for (int i = 0; i  < Planets.size(); ++i)
								{
									if (OTHER_SPACE_TYPE::SECTOR_DOWN == Planets.at(i).planetType)
									{
										ThePlayerShip.m_currentPlanetIndex = i;
									}
								}

								TargetPlanet = ThePlayerShip.m_currentPlanetIndex;
								Point targetPosition = Planets.at(ThePlayerShip.m_currentPlanetIndex).position;
								ThePlayerShip.position.setPosition(targetPosition.X(), targetPosition.Y(), ZORDER::PLAYER_OBJECT);
								break;
							}

						case PLANET_SCREEN_OPTIONS::SECTOR_DOWN:
							{
								CurrentGameState = GAME_STATE::PLANET_SELECTION_SCREEN;
								++CurrentSectorY; 
								if (CurrentSectorY >= UNIVERSE_HEIGHT)
								{
									CurrentSectorY = 0;
								}

								generateSector(currentSector(), StarField, Planets, TheHomeworld);
								generateMenuBasedOnCurrentSector(currentSector(), Planets, &m_menuModel, &m_viewConfiguration, &m_menuView);

								for (int i = 0; i  < Planets.size(); ++i)
								{
									if (OTHER_SPACE_TYPE::SECTOR_UP == Planets.at(i).planetType)
									{
										ThePlayerShip.m_currentPlanetIndex = i;
									}
								}
								TargetPlanet = ThePlayerShip.m_currentPlanetIndex;
								Point targetPosition = Planets.at(ThePlayerShip.m_currentPlanetIndex).position;
								ThePlayerShip.position.setPosition(targetPosition.X(), targetPosition.Y(), ZORDER::PLAYER_OBJECT);
								break;
							}

						case PLANET_SCREEN_OPTIONS::SCAN:
							{
								ExploredSectorToPlanetStatuses[currentSector()].at(ThePlayerShip.m_currentPlanetIndex).scanLevel++;
								ThePlayerShip.resetFuel(ThePlayerShip.m_fuel - SCAN_FUEL_COST);
								ThePlayerShip.resetSupplies(ThePlayerShip.m_supplies - SCAN_SUPPLIES_COST);
								TheHomeworld.countdown(SCAN_TIME_COST);
								generateMenuBasedOnPlanet(currentSector(), Planets, TargetPlanet, ThePlayerShip.m_currentPlanetIndex, &m_menuModel, &m_viewConfiguration, &m_menuView);

								break;
							}

						case PLANET_SCREEN_OPTIONS::RESUPPLY:
						case PLANET_SCREEN_OPTIONS::RESUPPLY_OFF_HOMEWORLD:
							{
								ThePlayerShip.resetFuel(FuelCapacities[ThePlayerShip.m_fuelCapacityLevel]);
								ThePlayerShip.resetSupplies(SupplyCapacities[ThePlayerShip.m_supplyCapacityLevel]);
								ThePlayerShip.resetCrew(CrewCapacities[ThePlayerShip.m_crewCapacityLevel]);
								TheHomeworld.countdown(RESUPPLY_TIME_COST);
								generateMenuBasedOnPlanet(currentSector(), Planets, TargetPlanet, ThePlayerShip.m_currentPlanetIndex, &m_menuModel, &m_viewConfiguration, &m_menuView);

								break;
							}

						case PLANET_SCREEN_OPTIONS::UPGRADE_FUEL_CAPACITY:
							{
								if (TheHomeworld.wealth > FuelUpgradeWealthRequirements[ThePlayerShip.m_fuelCapacityLevel])
								{
									TheHomeworld.wealth -= FuelUpgradeWealthRequirements[ThePlayerShip.m_fuelCapacityLevel];
									TheHomeworld.countdown(FUEL_UPGRADE_TIME_COST);
									ThePlayerShip.m_fuelCapacityLevel++;
									ThePlayerShip.resetFuel(FuelCapacities[ThePlayerShip.m_fuelCapacityLevel]);
									generateMenuBasedOnPlanet(currentSector(), Planets, TargetPlanet, ThePlayerShip.m_currentPlanetIndex, &m_menuModel, &m_viewConfiguration, &m_menuView);
								}

								break;
							}

						case PLANET_SCREEN_OPTIONS::UPGRADE_SUPPLY_CAPACITY:
							{
								if (TheHomeworld.wealth > SupplyUpgradeWealthRequirements[ThePlayerShip.m_supplyCapacityLevel])
								{
									TheHomeworld.wealth -= SupplyUpgradeWealthRequirements[ThePlayerShip.m_supplyCapacityLevel];
									TheHomeworld.countdown(SUPPLY_UPGRADE_TIME_COST);
									ThePlayerShip.m_supplyCapacityLevel++;
									ThePlayerShip.resetSupplies(SupplyCapacities[ThePlayerShip.m_supplyCapacityLevel]);
									generateMenuBasedOnPlanet(currentSector(), Planets, TargetPlanet, ThePlayerShip.m_currentPlanetIndex, &m_menuModel, &m_viewConfiguration, &m_menuView);
								}

								break;
							}

						case PLANET_SCREEN_OPTIONS::UPGRADE_CREW_CAPACITY:
							{
								if (TheHomeworld.wealth > CrewUpgradeWealthRequirements[ThePlayerShip.m_crewCapacityLevel])
								{
									TheHomeworld.wealth -= CrewUpgradeWealthRequirements[ThePlayerShip.m_crewCapacityLevel];
									TheHomeworld.countdown(CREW_UPGRADE_TIME_COST);
									ThePlayerShip.m_crewCapacityLevel++;
									ThePlayerShip.resetCrew(CrewCapacities[ThePlayerShip.m_crewCapacityLevel]);
									generateMenuBasedOnPlanet(currentSector(), Planets, TargetPlanet, ThePlayerShip.m_currentPlanetIndex, &m_menuModel, &m_viewConfiguration, &m_menuView);
								}

								break;
							}

						case PLANET_SCREEN_OPTIONS::BUILD_BASE:
							{
								if (BUILD_BASE_MINIMUM_SHIP_CREW_REQUIREMENT < ThePlayerShip.m_crew)
								{
									if (PLANET_BASE_STATUS::BASIC == ExploredSectorToPlanetStatuses[currentSector()].at(ThePlayerShip.m_currentPlanetIndex).base.baseLevel) 
									{
										ExploredSectorToPlanetStatuses[currentSector()].at(ThePlayerShip.m_currentPlanetIndex).base.baseLevel = PLANET_BASE_STATUS::UPGRADING;
									}
									else if (PLANET_LIFE_STATUS::UNINHABITABLE != ExploredSectorToPlanetStatuses[currentSector()].at(ThePlayerShip.m_currentPlanetIndex).lifeType)
									{
										ExploredSectorToPlanetStatuses[currentSector()].at(ThePlayerShip.m_currentPlanetIndex).base.baseLevel = PLANET_BASE_STATUS::BUILDING;
									}
									ExploredSectorToPlanetStatuses[currentSector()].at(ThePlayerShip.m_currentPlanetIndex).base.timeLeftToBuild = BUILD_BASE_TIME_COST;
									
									ThePlayerShip.resetFuel(ThePlayerShip.m_fuel - BUILD_BASE_FUEL_COST);
									ThePlayerShip.resetSupplies(ThePlayerShip.m_supplies - BUILD_BASE_SUPPLIES_COST);
									ThePlayerShip.resetCrew(ThePlayerShip.m_crew - BUILD_BASE_CREW_COST);
									generateMenuBasedOnPlanet(currentSector(), Planets, TargetPlanet, ThePlayerShip.m_currentPlanetIndex, &m_menuModel, &m_viewConfiguration, &m_menuView);
								}
								else
								{
									//TODO: Show error message about minimum ship crew requirement.
								}

								break;
							}
					}
				}
				break;
			}

		case GAME_STATE::SHOW_SHIP_MOVEMENT:
			{
				Point targetPlanetPosition = Planets.at(TargetPlanet).position;
				if (10 > calculateDistance(ThePlayerShip.position, targetPlanetPosition))
				{
					Point currentPlanetPosition = Planets.at(ThePlayerShip.m_currentPlanetIndex).position;
					double distance = calculateDistance(currentPlanetPosition, targetPlanetPosition);

					CurrentGameState = GAME_STATE::PLANET_SELECTION_SCREEN;
					ThePlayerShip.position.setPosition(targetPlanetPosition.X(), targetPlanetPosition.Y(), ZORDER::PLAYER_OBJECT);
					ThePlayerShip.m_currentPlanetIndex = TargetPlanet;
					generateMenuBasedOnCurrentSector(currentSector(), Planets, &m_menuModel, &m_viewConfiguration, &m_menuView);

					ThePlayerShip.resetFuel(ShipFuelBeforeLaunch - TotalShipFuelCost);
					ThePlayerShip.resetSupplies(ShipSuppliesBeforeLaunch - TotalShipSuppliesCost);
					
					TheHomeworld.countdown(TotalTimeCost);
				}
				else
				{
					TimeElapsed += timeElapsed;
					double percentInterpolated = TimeElapsed/1000.0;
					ThePlayerShip.resetFuel(ShipFuelBeforeLaunch - percentInterpolated*TotalShipFuelCost);
					ThePlayerShip.resetSupplies(ShipSuppliesBeforeLaunch - percentInterpolated*TotalShipSuppliesCost);
					//TheHomeworld.countdown(timeElapsed/1000.0 * TotalTimeCost);
					//TheHomeworld.timeUntilNova = TimeBeforeLaunch - percentInterpolated*TotalTimeCost;
					
					Point oldPlanetPosition = (Planets.at(ThePlayerShip.m_currentPlanetIndex).position);
					ThePlayerShip.position.setPosition(oldPlanetPosition.X() + (targetPlanetPosition.X() - oldPlanetPosition.X()) * percentInterpolated, oldPlanetPosition.Y() + (targetPlanetPosition.Y() - oldPlanetPosition.Y()) * percentInterpolated, ZORDER::PLAYER_OBJECT);
				}
				break;
			}
	}
	m_mouseRenderer->update(m_game.getInputState());
}

IRenderCommand * InGameState::draw()
{
	DrawRectangleCommand * cmd = new DrawRectangleCommand();
	DrawRectangleCommand * background = new DrawRectangleCommand();
	background->setRectangle(Point(0, 0), Rectangle(0, 0, 800, 600), 0x1a, 0x0f , 0x1d);
	for (std::vector<Star>::iterator iter = StarField.begin(); iter != StarField.end(); ++iter)
	{
		DrawRectangleCommand * star = new DrawRectangleCommand();
		star->setRectangle(iter->position, Rectangle(0, 0, 2, 2), iter->color.redness, iter->color.greenness, iter->color.blueness);
		background->add(star);
	}
	cmd->add(background);

	//CREATE HUD
	{
		DrawRectangleCommand * hud = new DrawRectangleCommand();
		hud->setRectangle(Point(0, 0, ZORDER::HUD), Rectangle(0, 0, 800, 60), 0x58, 0x97, 0x98);
		DrawRectangleCommand * innerHud = new DrawRectangleCommand();
		innerHud->setRectangle(Point(5, 5, ZORDER::HUD), Rectangle(0, 0, 790, 50), 0x2e, 0x77, 0x78);
		hud->add(innerHud);

		if (GAME_STATE::DEFEAT_SCREEN == CurrentGameState)
		{
			hud->add(DefeatReasonTextLabel.draw(DefeatReason));

			std::stringstream defeatText;
			defeatText << "You failed to evacuate the homeworld in time.";
			hud->add(DefeatTextLabel.draw(defeatText.str()));
		}
		else if (GAME_STATE::VICTORY_SCREEN == CurrentGameState)
		{
			hud->add(VictoryTextLabel.draw("You saved your homeworld's people."));
			hud->add(VictoryTextLabel2.draw("In the sky, you can see a start that shines more brightly than most."));
		}
		else
		{
			std::stringstream fuelText;
			fuelText.precision(3);
			fuelText << std::fixed;
			fuelText << "FUEL: " << ThePlayerShip.m_fuel << " / " << FuelCapacities[ThePlayerShip.m_fuelCapacityLevel];
			hud->add(FuelLabel.draw(fuelText.str()));

			std::stringstream suppliesText;
			suppliesText.precision(3);
			suppliesText << "SUPPLIES: " << ThePlayerShip.m_supplies << " / " << SupplyCapacities[ThePlayerShip.m_supplyCapacityLevel];
			hud->add(SuppliesLabel.draw(suppliesText.str()));

			std::stringstream wealthText;
			wealthText.precision(3);
			wealthText << std::fixed;
			wealthText << "HOMEWORLD WEALTH: " << TheHomeworld.wealth << " / " << WINNING_WEALTH;
			hud->add(HomeworldWealthLabel.draw(wealthText.str()));

			std::stringstream crewText;
			crewText << "CREW: " << ThePlayerShip.m_crew << " / " << CrewCapacities[ThePlayerShip.m_crewCapacityLevel];
			hud->add(CrewLabel.draw(crewText.str()));

			std::stringstream sectorText;
			sectorText << "SECTOR: " << currentSector();
			hud->add(CurrentSectorLabel.draw(sectorText.str()));

			std::stringstream countdownText;
			countdownText.precision(1);
			countdownText << std::fixed;
			countdownText << "TIME LEFT: " << TheHomeworld.timeUntilNova << " months";
			hud->add(NovaCountdownLabel.draw(countdownText.str()));
		}

		cmd->add(hud);
	}

	//RENDER PLANETS AND OTHER SPACE STUFF AND PLAYER SHIP
	{
		for (std::vector<Planet>::iterator iter = Planets.begin(); iter != Planets.end(); ++iter)
		{
			int planetType = iter->planetType;
			Point planetPosition(iter->position);
			SpriteImage * sprite = PlanetTypeToSprite[planetType];

			cmd->add(sprite->draw(planetPosition));
		}

		if (GAME_STATE::DEFEAT_SCREEN != CurrentGameState && GAME_STATE::VICTORY_SCREEN != CurrentGameState)
		{
			cmd->add(OrientationToSprite[ThePlayerShip.orientation]->draw(ThePlayerShip.position));
		}
	}

	//RENDER FOREGROUND FOR MENU
	{
		DrawRectangleCommand * foreground = new DrawRectangleCommand();
		if (m_menuView)
		{
			if (GAME_STATE::PLANET_SCREEN == CurrentGameState)
			{
				int foregroundX(50);
				int foregroundY(80);
				int foregroundWidth(700);
				int foregroundHeight(600 - 60 - foregroundY);
				foreground->setRectangle(Point(foregroundX, foregroundY, ZORDER::HUD), Rectangle(0, 0, foregroundWidth, foregroundHeight), 0x58, 0x97, 0x98);
				DrawRectangleCommand * innerForeground = new DrawRectangleCommand();
				innerForeground->setRectangle(Point(foregroundX + 5, foregroundY + 5, ZORDER::BG - 1), Rectangle(0, 0, foregroundWidth - 10, foregroundHeight - 10), 0x78, 0x2e, 0x2e);
				foreground->add(innerForeground);
				foreground->add(PlanetTypeToLargeSprite[Planets.at(TargetPlanet).planetType]->draw(Point(400, 300, ZORDER::HUD)));


				std::stringstream menuTitleText;

				std::string whichOneDidYouPick = PlanetTypeToSprite[Planets.at(TargetPlanet).planetType]->m_imageName;
				switch(Planets.at(TargetPlanet).planetType)
				{
					case PLANET_TYPE::METAL:
					case PLANET_TYPE::HOMEWORLD:
					case PLANET_TYPE::RED:
					case PLANET_TYPE::GAS:
					case PLANET_TYPE::SWIRLY1:
					case PLANET_TYPE::SWIRLY2:
					case PLANET_TYPE::SWIRLY3:
					{
						menuTitleText << "Planet: ";
						if (PLANET_SCAN_STATUS::NONE == ExploredSectorToPlanetStatuses[currentSector()].at(TargetPlanet).scanLevel)
						{
							menuTitleText << "Unexplored "; 
						}
						else if (PLANET_LIFE_STATUS::UNINHABITABLE == ExploredSectorToPlanetStatuses[currentSector()].at(TargetPlanet).lifeType)
						{
							menuTitleText << "Uninhabitable ";
						}
						menuTitleText << whichOneDidYouPick;

						break;
					}

					case OTHER_SPACE_TYPE::THE_HOMEWORLD:
					{
						menuTitleText << "Our Homeworld";
						break;
					}

					case OTHER_SPACE_TYPE::SECTOR_UP:
					case OTHER_SPACE_TYPE::SECTOR_DOWN:
					case OTHER_SPACE_TYPE::SECTOR_LEFT:
					case OTHER_SPACE_TYPE::SECTOR_RIGHT:
					{
						menuTitleText << "Jump Point For A Nearby System";
						break;
					}
				}
				foreground->add(PlanetMenuTitle.draw(menuTitleText.str()));

				//DISPLAY VITAL INFO
				{
					foreground->add(ScanResultsTitle.draw("SCAN DATA:"));
					int scanLevel = ExploredSectorToPlanetStatuses[currentSector()].at(TargetPlanet).scanLevel;
					std::stringstream scanResultsResourcesText;
					scanResultsResourcesText << "Resources: " << ScanLevelToResourceStatusToText[scanLevel][ExploredSectorToPlanetStatuses[currentSector()].at(TargetPlanet).resourceAvailability];;

					std::stringstream scanResultsLifeText;
					scanResultsLifeText << "Life: " << ScanLevelToLifeStatusToText[scanLevel][ExploredSectorToPlanetStatuses[currentSector()].at(TargetPlanet).lifeType];
					
					foreground->add(ScanResultsResourcesLabel.draw(scanResultsResourcesText.str()));
					foreground->add(ScanResultsLifeLabel.draw(scanResultsLifeText.str()));
				}

				//DISPLAY BASE INFO
				{
				}

			}
			else if (GAME_STATE::VICTORY_SCREEN == CurrentGameState || GAME_STATE::DEFEAT_SCREEN == CurrentGameState)
			{
				foreground->add(SuperNovaSprite->draw(Point(400, 300, ZORDER::HUD)));
			}
			foreground->add(m_menuView->draw());
		}
		cmd->add(foreground);
	}

	cmd->add(m_mouseRenderer->draw());
	return cmd;
}

void InGameState::onExit()
{
	delete m_mouseRenderer;
	m_mouseRenderer = 0;
	delete m_menuView;
	m_menuView = 0;
	delete m_viewConfiguration;
	m_viewConfiguration = 0;
	delete m_menuModel;
	m_menuModel = 0;

}

void InGameState::onSuspend()
{
	m_stateSuspended = true;
}

void InGameState::onUnsuspend()
{
	m_stateSuspended = false;
}
