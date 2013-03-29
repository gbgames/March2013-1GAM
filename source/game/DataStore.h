#ifndef GB_DATASTORE_H
#define GB_DATASTORE_H

#include "IDataStore.h"

#include <map>

typedef std::map<ObjectID, IComponent *>  ComponentCollectionByID;

class DataStore : public IDataStore
{
	public:
		DataStore();
		virtual ~DataStore();

		virtual int objectCount() const;
		virtual int totalObjectCount() const;
		virtual ObjectID getNextAvailableObjectID() const;
		virtual void createObject(ObjectID objectID); 
		virtual void assignName(ObjectID objectID, std::string objectName);
		virtual ObjectID getObjectID(std::string objectName);
		virtual void destroyObject(ObjectID objectID);

		virtual void registerComponentType(std::string componentName);
		virtual void addComponent(ObjectID objID , IComponent * component);
		virtual void addComponent(std::string objectName, IComponent * component);
		virtual IComponent * getComponent(ObjectID objID, std::string componentName);
		virtual IComponent * getComponent(std::string objectName, std::string componentName);
		virtual void removeComponent(ObjectID, std::string componentID);

		virtual ObjectIDs getObjectsWithComponent(std::string componentName);
		virtual bool objectExists(ObjectID objectID);

	private:
		int getNextAvailableComponentID() const;
		void throwError(std::string errorMessage);

	public:
		int m_objectCount;
		int m_totalObjectCount;
		int m_nextComponentID;
		ObjectIDs m_objectIDs;
		std::map<std::string, ObjectID> m_nameToObjectID;
		std::map<std::string, int> m_componentNamesToComponentID;
		std::map<int, ComponentCollectionByID> m_componentTypeToComponentCollection;
};

#endif
