#include "ObjectFactory.h"

#include "DataStore.h"
#include "ComponentTypes.h"
#include "MovementComponent.h"
//#include "PathfindingComponent.h"
//#include "PositionComponent.h"
//#include "RenderableComponent.h"
//#include "SteeringComponent.h"
//#include "TimerComponent.h" // Event-related?
//#include "UIComponent.h"

ObjectFactory::ObjectFactory() 
{
	//TODO: Should be dynamically initialized. Allow game developer
	//to provide own component creation factory methods.
	m_componentNameToEnumValue["Attack"] = ComponentType::Attack;
	m_componentNameToEnumValue["Brain"] = ComponentType::Brain;
	m_componentNameToEnumValue["Collector"] = ComponentType::Collector;
	m_componentNameToEnumValue["Collectable"] = ComponentType::Collectable;
	m_componentNameToEnumValue["Conquerable"] = ComponentType::Conquerable;
	m_componentNameToEnumValue["Conqueror"] = ComponentType::Conqueror;
	m_componentNameToEnumValue["Health"] = ComponentType::Health;
	m_componentNameToEnumValue["Life"] = ComponentType::Life;
	m_componentNameToEnumValue["MakesSlow"] = ComponentType::MakesSlow;
	m_componentNameToEnumValue["Movement"] = ComponentType::Movement;
	m_componentNameToEnumValue["Obstacle"] = ComponentType::Obstacle;
	m_componentNameToEnumValue["Pathfinding"] = ComponentType::Pathfinding;
	m_componentNameToEnumValue["Position"] = ComponentType::Position;
	m_componentNameToEnumValue["Renderable"] = ComponentType::Renderable;
	m_componentNameToEnumValue["Resources"] = ComponentType::Resources;
	m_componentNameToEnumValue["Steering"] = ComponentType::Steering;
	m_componentNameToEnumValue["SummoningQueue"] = ComponentType::SummoningQueue;
	m_componentNameToEnumValue["SusceptibleToSlow"] = ComponentType::SusceptibleToSlow;
	m_componentNameToEnumValue["Targetable"] = ComponentType::Targetable;
	m_componentNameToEnumValue["Targeting"] = ComponentType::Targeting;
	m_componentNameToEnumValue["Team"] = ComponentType::Team;
	m_componentNameToEnumValue["Timer"] = ComponentType::Timer;
	m_componentNameToEnumValue["Trail"] = ComponentType::Trail;
	m_componentNameToEnumValue["TrailGenerator"] = ComponentType::TrailGenerator;
	m_componentNameToEnumValue["UI"] = ComponentType::UI;
	m_componentNameToEnumValue["Weapon"] = ComponentType::Weapon;
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::configureObjectTemplate(std::string templateName, std::list<ComponentData> componentConfigurations)
{
	m_templateNameToComponentConfigurations[templateName] = componentConfigurations;
}

std::list<IComponent *> ObjectFactory::createObjectFromTemplate(std::string templateName)
{
	std::list<IComponent *> components;
	std::list<ComponentData> componentConfigurations = m_templateNameToComponentConfigurations[templateName];

	for(std::list<ComponentData>::iterator iter = componentConfigurations.begin();
			iter != componentConfigurations.end();
			++iter)
	{
		components.push_back(createComponent(iter->first, iter->second));
	}

	return components;
}

IDataStore * ObjectFactory::createDataStore()
{
	DataStore * dataStore = new DataStore();
	dataStore->registerComponentType("Attack");
	dataStore->registerComponentType("Brain");
	dataStore->registerComponentType("Collector");
	dataStore->registerComponentType("Collectable");
	dataStore->registerComponentType("Conquerable");
	dataStore->registerComponentType("Conqueror");
	dataStore->registerComponentType("Health");
	dataStore->registerComponentType("Life");
	dataStore->registerComponentType("MakesSlow");
	dataStore->registerComponentType("Movement");
	dataStore->registerComponentType("Obstacle");
	dataStore->registerComponentType("Pathfinding");
	dataStore->registerComponentType("Position");
	dataStore->registerComponentType("Renderable");
	dataStore->registerComponentType("Resources");
	dataStore->registerComponentType("Steering");
	dataStore->registerComponentType("Steering");
	dataStore->registerComponentType("SummoningQueue");
	dataStore->registerComponentType("SusceptibleToSlow");
	dataStore->registerComponentType("Targetable");
	dataStore->registerComponentType("Targeting");
	dataStore->registerComponentType("Team");
	dataStore->registerComponentType("Timer");
	dataStore->registerComponentType("Trail");
	dataStore->registerComponentType("TrailGenerator");
	dataStore->registerComponentType("UI");
	dataStore->registerComponentType("Weapon");
	return dataStore;
}

IComponent * ObjectFactory::createComponent(std::string componentName, Parameters parameters)
{
	IComponent * component(0);
	/*
	switch (m_componentNameToEnumValue[componentName])
	{
		case ComponentType::Attack:
		{
			ObjectID attackerID = parameters.at(0).intData;
			ObjectID targetID = parameters.at(1).intData;
			int damage = parameters.at(2).intData;

			CommandConfigurations commands;
			int numCommands = parameters.at(3).intData;

			int parameterIndex = 4;
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

			float impactRange = parameters.at(parameterIndex++).floatData;
			float isAreaEffect = parameters.at(parameterIndex++).boolData;
			float areaEffectRadius = parameters.at(parameterIndex++).floatData;


			component = new AttackComponent(attackerID, targetID, damage, commands, impactRange, isAreaEffect, areaEffectRadius);
		}
		break;

		case ComponentType::Brain:
		{
			int maxCounter = parameters.at(0).intData;
			std::vector<std::string> goalEvaluatorNames;
			for (unsigned int i = 1; i <  parameters.size(); ++i)			
			{
				goalEvaluatorNames.push_back(parameters.at(i).stringData);
			}
			
			component = new BrainComponent(maxCounter, goalEvaluatorNames);
		}
		break;

		case ComponentType::Collector:
		{
			std::vector<Item::ID> itemList;
			for (std::vector<Parameter>::iterator iter = parameters.begin();
							iter != parameters.end();
							++iter)
			{
				itemList.push_back(iter->intData);
			}
			component = new CollectorComponent(itemList);
		}
		break;

		case ComponentType::Collectable:
		{
			Item::ID itemID = parameters.at(0).intData;
			CommandConfigurations commands;
			int numCommands = parameters.at(1).intData;

			int parameterIndex = 2;
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
			int reactivationTime = parameters.at(parameterIndex++).intData;

			component = new CollectableComponent(itemID, commands, reactivationTime);
		}
		break;

		case ComponentType::Conquerable:
		{
			component = new ConquerableComponent(parameters.at(0).stringData);
		}
		break;

		case ComponentType::Conqueror:
		{
			std::map<std::string, std::string> conquerableTransitionMapping;
			for (Parameters::iterator iter = parameters.begin();
							iter != parameters.end();)
			{
				std::string key(iter->stringData);
				++iter;
				std::string value(iter->stringData);
				conquerableTransitionMapping.insert(std::pair<std::string, std::string>(key, value));
				++iter;
			}
			component = new ConquerorComponent(conquerableTransitionMapping);
		}
		break;

		case ComponentType::Health:
		{
			int health = parameters.at(0).intData;

			component = new HealthComponent(health);
		}
		break;

		case ComponentType::Life:
		{
			component = new LifeComponent(parameters.at(0).intData);
		}
		break;

		case ComponentType::MakesSlow:
		{
			component = new MakesSlowComponent(parameters.at(0).floatData);
		}
		break;

		case ComponentType::Movement:
		{
			Point directionVector(parameters.at(1).floatData, parameters.at(2).floatData, parameters.at(3).floatData);
			component = new MovementComponent(parameters.at(0).floatData, directionVector); 
		}
		break;

		case ComponentType::Obstacle:
		{
			component = new ObstacleComponent(parameters.at(0).floatData);
		}
		break;

		case ComponentType::Position:
		{
			component = new PositionComponent(parameters.at(0).floatData, parameters.at(1).floatData, parameters.at(2).floatData); 
		}
		break;

		case ComponentType::Renderable:
		{
			component = new RenderableComponent(parameters.at(0).stringData); 
		}
		break;

		case ComponentType::Resources:
		{
			component = new ResourcesComponent(parameters.at(0).intData); 
		}
		break;

		case ComponentType::Steering:
		{
			component = new SteeringComponent();
		}
		break;

		case ComponentType::SummoningQueue:
		{
			int maxQueueSize(parameters.at(0).intData);
			std::vector<TemplateIDAndTime> summonQueueConfiguration;
			int maxConfigurationSize = parameters.at(1).intData;
			int parameterIndex = 2;
			for (int index = 0; index < maxConfigurationSize; ++index)
			{
				std::string templateID(parameters.at(parameterIndex++).stringData);
				int timeToSummon(parameters.at(parameterIndex++).intData);
				summonQueueConfiguration.push_back(TemplateIDAndTime(templateID, timeToSummon));
			}
			component = new SummoningQueueComponent(maxQueueSize, summonQueueConfiguration);
		}
		break;


		case ComponentType::SusceptibleToSlow:
		{
			component = new SusceptibleToSlowComponent();
		}
		break;

		case ComponentType::Targetable:
		{
			component = new TargetableComponent(parameters.at(0).stringData);
		}
		break;	

		case ComponentType::Targeting:
		{
			std::vector<std::string> targetList;
			for (std::vector<Parameter>::iterator iter = parameters.begin();
							iter != parameters.end();
							++iter)
			{
				targetList.push_back(iter->stringData);
			}
			component = new TargetingComponent(targetList);
		}
		break;
		
		case ComponentType::Team:
		{
			component = new TeamComponent(parameters.at(0).stringData);
		}
		break;	

		case ComponentType::Timer:
		{
			int timeToSet = parameters.at(0).intData;
			bool armedStatus = parameters.at(1).boolData;
			CommandConfigurations commands;
			int numCommands = parameters.at(2).intData;

			int parameterIndex = 3;
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

			bool isOneShot(parameters.at(parameterIndex++).boolData);
			int timeToReload(parameters.at(parameterIndex).intData);

			component = new TimerComponent(timeToSet, armedStatus, commands, isOneShot, timeToReload);
		}
		break;	

		case ComponentType::Trail:
		{
			component = new TrailComponent(parameters.at(0).stringData);
		}
		break;	

		case ComponentType::TrailGenerator:
		{
			component = new TrailGeneratorComponent(parameters.at(0).stringData, parameters.at(1).intData);
		}
		break;	

		case ComponentType::UI:
		{
			Rectangle area(parameters.at(0).floatData, parameters.at(1).floatData, parameters.at(2).floatData, parameters.at(3).floatData);
			Point hotSpot(parameters.at(4).floatData, parameters.at(5).floatData);
			std::string command(parameters.at(6).stringData);

			component = new UIComponent(area, hotSpot, command);
		}
		break;	

		case ComponentType::Weapon:
		{
			float range = parameters.at(0).floatData;
			int damage = parameters.at(1).intData;
			int coolDownTime = parameters.at(2).intData;

			CommandConfigurations commands;
			int numCommands = parameters.at(3).intData;

			int parameterIndex = 4;
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

			float impactRange = parameters.at(parameterIndex++).floatData;
			std::string attackRenderable = parameters.at(parameterIndex++).stringData;
			float attackSpeed = parameters.at(parameterIndex++).floatData;
			float isAreaEffect = parameters.at(parameterIndex++).boolData;
			float areaEffectRadius = parameters.at(parameterIndex++).floatData;

			component = new WeaponComponent(range, damage, coolDownTime, commands, impactRange, attackRenderable, attackSpeed, isAreaEffect, areaEffectRadius);
		}
		break;

		case ComponentType::Pathfinding:
		{
			unsigned int maxPathSegments(static_cast<unsigned int>(parameters.at(0).intData));
			std::vector<Graph::type> terrainData;
			for (size_t i = 1; i < parameters.size(); ++i)
			{
				terrainData.push_back(parameters.at(i).intData);
			}
			PathfindingComponent * pathfindingComponent = new PathfindingComponent(terrainData);
			pathfindingComponent->setMaxPathSegments(maxPathSegments);

			component = pathfindingComponent;
		}
		break;

		default:
		{
			throw 1;
		}
	}
	*/
	
	return component;
}
