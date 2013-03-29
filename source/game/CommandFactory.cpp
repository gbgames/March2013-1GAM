#include "CommandFactory.h"

#include "NullCommand.h"
#include "PauseGameCommand.h"
#include "QuitApplicationCommand.h"

#include <iostream>

CommandFactory::CommandFactory(IObjectFactory & objectFactory) : 
	m_objectFactory(objectFactory)
{
	m_commandNameToID["Quit"] = Command::QUIT_APPLICATION;
	m_commandNameToID["PauseGame"] = Command::PAUSE_GAME;
	m_commandNameToID["PlayerDefeated"] = Command::PLAYER_DEFEATED;
	m_commandNameToID["PlayerVictorious"] = Command::PLAYER_VICTORIOUS;

	m_commandNameToID["CreateObjectFromTemplate"] = Command::CREATE_OBJECT_FROM_TEMPLATE_COMMAND;

	m_commandNameToID["CreateAttack"] = Command::CREATE_ATTACK_COMMAND;

	m_commandNameToID["ActivateCollectable"] = Command::ACTIVATE_COLLECTABLE_COMMAND;
	m_commandNameToID["DeactivateCollectable"] = Command::DEACTIVATE_COLLECTABLE_COMMAND;
	m_commandNameToID["ClearTower"] = Command::CLEAR_TOWER_COMMAND;
	m_commandNameToID["DestroyObject"] = Command::DESTROY_OBJECT_COMMAND;
	m_commandNameToID["KillEntity"] = Command::KILL_ENTITY_COMMAND;
	m_commandNameToID["PurchaseEntity"] = Command::PURCHASE_ENTITY_COMMAND;
	m_commandNameToID["ResetAndArmTimerComponent"] = Command::RESET_AND_ARM_TIMER_COMPONENT;
	m_commandNameToID["UpdatePlayerResources"] = Command::UPDATE_PLAYER_RESOURCES_COMMAND;
	m_commandNameToID["UpdateTeamResources"] = Command::UPDATE_TEAM_RESOURCES_COMMAND;
	m_commandNameToID["UpgradeWeapon"] = Command::UPGRADE_WEAPON_COMMAND;
}

CommandFactory::~CommandFactory()
{
}

ICommand * CommandFactory::createCommand(std::string cmdName, ILevelInstance * level, Parameters parameters)
{
	ICommand * cmd(0);
	/*
	IDataStore * dataStore = level->dataStore();
	switch (m_commandNameToID[cmdName])
	{
		case Command::CREATE_OBJECT_FROM_TEMPLATE_COMMAND:
			{
				std::string templateID(parameters.at(0).stringData);
				Point positionData(parameters.at(1).floatData, parameters.at(2).floatData, parameters.at(3).floatData);
				ObjectID existingObjectID(parameters.at(4).intData);
				cmd = new CreateObjectFromTemplateCommand(dataStore, m_objectFactory, templateID, positionData, existingObjectID);
			}
			break;

		case Command::CREATE_ATTACK_COMMAND:
			{
				Point positionData(parameters.at(0).floatData, parameters.at(1).floatData, parameters.at(2).floatData);
				ObjectID attackerID(parameters.at(3).intData);
				ObjectID targetID(parameters.at(4).intData);
				int damage(parameters.at(5).intData);

				int numCommands = parameters.at(6).intData;

				CommandConfigurations commands;
				int parameterIndex = 7;
				for (int index = 0; index < numCommands; ++index)
				{
					std::string commandID = parameters.at(parameterIndex++).stringData;

					Parameters commandParameters;
					int numCommandParameters = parameters.at(parameterIndex++).intData;
					for (int commandParameterIndex = 0; commandParameterIndex < numCommandParameters; ++commandParameterIndex)
					{
						Parameter commandParameter = parameters.at(parameterIndex++);
						commandParameters.push_back(commandParameter);
					}

					commands.push_back(CommandConfiguration(commandID, commandParameters));
				}

				float impactRange(parameters.at(parameterIndex++).floatData);
				std::string renderable(parameters.at(parameterIndex++).stringData);
				float attackSpeed(parameters.at(parameterIndex++).floatData);
				bool isAreaEffect(parameters.at(parameterIndex++).boolData);
				float areaEffectRadius(parameters.at(parameterIndex++).floatData);

				cmd = new CreateAttackCommand(level, m_objectFactory, positionData, attackerID, targetID, damage, commands, impactRange, renderable, attackSpeed, isAreaEffect, areaEffectRadius);
			}
			break;

		case Command::ACTIVATE_COLLECTABLE_COMMAND:
			{
				ObjectID entityID(parameters.at(0).intData);
				cmd = new ActivateCollectableCommand(dataStore, m_objectFactory, entityID);
			}
			break;
		
		case Command::DEACTIVATE_COLLECTABLE_COMMAND:
			{
				ObjectID entityID(parameters.at(0).intData);
				cmd = new DeactivateCollectableCommand(dataStore, m_objectFactory, entityID);
			}
			break;


		case Command::CLEAR_TOWER_COMMAND:
			{
				ObjectID towerID(parameters.at(0).intData);
				ObjectID conquerorID(parameters.at(1).intData);
				cmd = new ClearTowerCommand(dataStore, m_objectFactory, towerID, conquerorID);
			}
			break;

		case Command::DESTROY_OBJECT_COMMAND:
			{
				ObjectID objectID(parameters.at(0).intData);
				cmd = new DestroyObjectCommand(dataStore, m_objectFactory, objectID);
			}
			break;


		case Command::KILL_ENTITY_COMMAND:
			{
				ObjectID entityID(parameters.at(0).intData);
				cmd = new KillEntityCommand(dataStore, m_objectFactory, entityID);
			}
			break;
		
		case Command::PURCHASE_ENTITY_COMMAND:
			{
				ObjectID queueOwner(parameters.at(0).intData);
				std::string playerID(parameters.at(1).stringData);
				std::string entity(parameters.at(2).stringData);
				cmd = new PurchaseEntityCommand(level, m_objectFactory, queueOwner, playerID, entity);
			}
			break;


		case Command::RESET_AND_ARM_TIMER_COMPONENT:
			{
				ObjectID entityID(parameters.at(0).intData);
				int time = parameters.at(1).intData;
				bool armedStatus = parameters.at(2).boolData;
				cmd = new ResetAndArmTimerComponentCommand(dataStore, m_objectFactory, entityID, time, armedStatus);
			}
			break;


		case Command::UPGRADE_WEAPON_COMMAND:
			{
				ObjectID collectorID(parameters.at(0).intData);
				ObjectID collectableID(parameters.at(1).intData);
				cmd = new UpgradeWeaponCommand(dataStore, m_objectFactory, collectorID, collectableID);
			}
			break;

		case Command::UPDATE_PLAYER_RESOURCES_COMMAND:
			{
				std::string playerID(parameters.at(0).stringData);
				int resources(parameters.at(1).intData);
				cmd = new UpdatePlayerResourcesCommand(dataStore, m_objectFactory, playerID, resources);
			}
			break;

		case Command::UPDATE_TEAM_RESOURCES_COMMAND:
			{
				int resources(parameters.at(0).intData);
				ObjectID collectorID(parameters.at(1).intData);
				cmd = new UpdateTeamResourcesCommand(level, m_objectFactory, resources, collectorID);
			}
			break;


		default:
			{
				cmd = new NullCommand();
			}
			break;
	}
	*/

	return cmd;
}

ICommand * CommandFactory::createApplicationCommand(std::string cmdName, IGame & game)
{
	ICommand * cmd(0);
	switch (m_commandNameToID[cmdName])
	{
		case Command::QUIT_APPLICATION:
			{
				cmd = new QuitApplicationCommand(game);
			}
			break;

		case Command::PAUSE_GAME:
			{
				cmd = new PauseGameCommand(game);
			}
			break;

/*
		case Command::PLAYER_DEFEATED:
			{
				cmd = new PlayerDefeatedCommand(game);
			}
			break;

		case Command::PLAYER_VICTORIOUS:
			{
				cmd = new PlayerVictoriousCommand(game);
			}
			break;

*/

		default:
			{
				cmd = new NullCommand();
			}
			break;
	}

	return cmd;
}
