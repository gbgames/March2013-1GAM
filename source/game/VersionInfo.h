#ifndef GB_VERSIONINFO_H
#define GB_VERSIONINFO_H

#include "IVersionInfo.h"

class VersionInfo : public IVersionInfo
{
	public:
		VersionInfo();
		virtual ~VersionInfo();
	
	public:
		virtual int major();
		virtual int minor();
		virtual int patch();
		virtual std::string releaseType();
		virtual std::string developmentPhase();
		virtual DemoStatus demoStatus();
};

#endif
