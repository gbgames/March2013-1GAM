#ifndef GB_IDATASTORE_H
#define GB_IDATASTORE_H

#include "ObjectID.h"

#include <string>
#include <vector>

class IComponent;

class IDataStore
{
	public:
		virtual ~IDataStore() {}

	public:
		virtual int objectCount() const = 0;
		virtual int totalObjectCount() const = 0;
		virtual ObjectID getNextAvailableObjectID() const = 0;
		virtual void createObject(ObjectID objectID) = 0;
		virtual void assignName(ObjectID objectID, std::string objectName) = 0;
		virtual ObjectID getObjectID(std::string objectName) = 0;
		virtual void destroyObject(ObjectID objectID) = 0;

		virtual void registerComponentType(std::string componentName) = 0;
		virtual void addComponent(ObjectID objID , IComponent * component) = 0;
		virtual void addComponent(std::string objectName, IComponent * component) = 0;
		virtual IComponent * getComponent(ObjectID objID, std::string componentName) = 0;
		virtual IComponent * getComponent(std::string objectName, std::string componentName) = 0;
		virtual void removeComponent(ObjectID objectID, std::string componentID) = 0;

		virtual ObjectIDs getObjectsWithComponent(std::string) = 0;
		virtual bool objectExists(ObjectID objectID) = 0;
};

#endif
