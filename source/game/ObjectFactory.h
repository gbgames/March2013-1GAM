#ifndef GB_OBJECTFACTORY_H
#define GB_OBJECTFACTORY_H

#include "IObjectFactory.h"

#include <string>
#include <map>

class ObjectFactory : public IObjectFactory
{
	public:
		ObjectFactory();
		virtual ~ObjectFactory();

	public:
		virtual void configureObjectTemplate(std::string templateName, std::list<ComponentData> componentConfiguration);
		virtual std::list<IComponent *> createObjectFromTemplate(std::string templateName);
		virtual IDataStore * createDataStore(); 

		virtual IComponent * createComponent(std::string componentName, Parameters parameters);

	public:
		std::map<std::string, int> m_componentNameToEnumValue;
		std::map<std::string, std::list<ComponentData> > m_templateNameToComponentConfigurations;
};

#endif
