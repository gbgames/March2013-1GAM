#include "DataStore.h"

#include "IComponent.h"

#include <algorithm>

DataStore::DataStore() :
	m_objectCount(0),
	m_totalObjectCount(0),
	m_nextComponentID(1)
{
}

DataStore::~DataStore()
{
	for (std::map<int, ComponentCollectionByID>::iterator iter = m_componentTypeToComponentCollection.begin();
			iter != m_componentTypeToComponentCollection.end();
			++iter)
	{
		for (ComponentCollectionByID::iterator componentIter = iter->second.begin();
				componentIter != iter->second.end();
				++componentIter)
		{
			delete componentIter->second;
		}
	}
}

int DataStore::objectCount() const
{
	return m_objectCount;
}

int DataStore::totalObjectCount() const
{
	return m_totalObjectCount;
}

ObjectID DataStore::getNextAvailableObjectID() const
{
	return totalObjectCount() + 1;
}

void DataStore::createObject(ObjectID objectID)
{
	if (!objectExists(objectID))
	{
		m_objectIDs.push_back(objectID);
		++m_objectCount;
		++m_totalObjectCount;
	}
	else
	{
		throwError("Tried to create object using an existing object's unique ID.");
	}
}

void DataStore::assignName(ObjectID objectID, std::string objectName)
{
	m_nameToObjectID[objectName] = objectID;
}

ObjectID DataStore::getObjectID(std::string objectName)
{
	return m_nameToObjectID[objectName];
}

void DataStore::destroyObject(ObjectID objectID)
{
	ObjectIDs::iterator objectIter = std::find(m_objectIDs.begin(), m_objectIDs.end(), objectID);
	if (m_objectIDs.end() != objectIter)
	{
		for(std::map<int, ComponentCollectionByID>::iterator iter = m_componentTypeToComponentCollection.begin();
						iter != m_componentTypeToComponentCollection.end();
						++iter)
		{
			ComponentCollectionByID::iterator collection = iter->second.find(objectID);
			if (iter->second.end() != collection)
			{
				delete (collection->second);
				iter->second.erase(collection);
			}
		}

		m_objectIDs.erase(objectIter);
		--m_objectCount;
	}
}

void DataStore::registerComponentType(std::string componentName)
{
	if ("" != componentName)
	{
		m_componentNamesToComponentID[componentName] = getNextAvailableComponentID();
		++m_nextComponentID;
	}
}

void DataStore::addComponent(ObjectID objectID, IComponent  * component)
{
	if (objectExists(objectID) &&
		m_componentNamesToComponentID.end() != 
			m_componentNamesToComponentID.find(component->name()))
	{
		m_componentTypeToComponentCollection[m_componentNamesToComponentID[component->name()]][objectID] = component;
	}
	else
	{
		throwError("Either invalid objectID or unregistered component");
	}
}

void DataStore::addComponent(std::string objectName, IComponent * component)
{
	addComponent(getObjectID(objectName), component);
}


IComponent * DataStore::getComponent(ObjectID objectID, std::string componentName)
{
	IComponent * component(0);

	std::map<int, ComponentCollectionByID>::iterator iter = m_componentTypeToComponentCollection.find(m_componentNamesToComponentID[componentName]);

	if (m_componentTypeToComponentCollection.end() != iter)
	{
		ComponentCollectionByID::iterator collection = iter->second.find(objectID);
		if (iter->second.end() != collection)
		{
			component = collection->second;
		}
	}

	return component;
}

IComponent * DataStore::getComponent(std::string objectName, std::string componentName)
{
	return getComponent(getObjectID(objectName), componentName);
}

void DataStore::removeComponent(ObjectID objectID, std::string componentName)
{
	ObjectIDs::iterator objectIter = std::find(m_objectIDs.begin(), m_objectIDs.end(), objectID);
	if (m_objectIDs.end() != objectIter)
	{
		ComponentCollectionByID & componentCollection = m_componentTypeToComponentCollection[m_componentNamesToComponentID[componentName]];
		delete componentCollection[objectID];
		componentCollection.erase(componentCollection.find(objectID));
	}
}

ObjectIDs DataStore::getObjectsWithComponent(std::string componentName)
{
	ObjectIDs objIDCollection;

	std::map<int, ComponentCollectionByID>::iterator iter = m_componentTypeToComponentCollection.find(m_componentNamesToComponentID[componentName]);

	if (m_componentTypeToComponentCollection.end() != iter)
	{
		for (ComponentCollectionByID::iterator collection = iter->second.begin();
				collection != iter->second.end();
				++collection)
		{
			objIDCollection.push_back(collection->first);
		}
	}

	return objIDCollection;
}

bool DataStore::objectExists(ObjectID objectID)
{
	return (m_objectIDs.end() !=
			std::find(m_objectIDs.begin(), m_objectIDs.end(), objectID));
}

int DataStore::getNextAvailableComponentID() const
{
	return m_nextComponentID;
}

void DataStore::throwError(std::string errorMessage)
{
	throw errorMessage;
}
