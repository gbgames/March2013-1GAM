#ifndef GB_IVERSIONINFO_H
#define GB_VERSIONINFO_H

#include <string>

namespace Version
{
	enum DemoStatuses 
	{
		DEMO,
		FULL
	};
};

typedef int DemoStatus;

class IVersionInfo
{
	public:
		virtual ~IVersionInfo() {}
	
	public:
		virtual int major() = 0;
		virtual int minor() = 0;
		virtual int patch() = 0;
		virtual std::string releaseType() = 0;
		virtual std::string developmentPhase() = 0;
		virtual DemoStatus demoStatus() = 0;
};


#endif
