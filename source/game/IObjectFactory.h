#ifndef GB_IOBJECTFACTORY_H
#define GB_IOBJECTFACTORY_H

#include "Parameters.h" // Utilities

#include <list>
#include <string>

class IComponent;
class IDataStore;

typedef std::pair<std::string, Parameters> ComponentData;

class IObjectFactory
{
	public:
		virtual ~IObjectFactory() {}

	public:
		virtual void configureObjectTemplate(std::string templateName, std::list<ComponentData> componentConfigurations) = 0;
		virtual std::list<IComponent *> createObjectFromTemplate(std::string templateName) = 0;
		virtual IDataStore * createDataStore() = 0; 

		virtual IComponent * createComponent(std::string componentName, Parameters parameters) = 0;
};

#endif
