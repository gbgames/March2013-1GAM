#include "VersionInfo.h"

#include "OGAMVersion.h"

VersionInfo::VersionInfo()
{
}

VersionInfo::~VersionInfo()
{
}

int VersionInfo::major()
{
	return Version::OGAM_VERSION_MAJOR;
}

int VersionInfo::minor()
{
	return Version::OGAM_VERSION_MINOR;
}

int VersionInfo::patch()
{
	return Version::OGAM_VERSION_PATCH;
}

std::string VersionInfo::releaseType()
{
	return Version::OGAM_RELEASE_TYPE;
}

std::string VersionInfo::developmentPhase()
{
	return Version::OGAM_VERSION_STATUS;
}

DemoStatus VersionInfo::demoStatus()
{
	DemoStatus demoStatus = Version::FULL;

	if ("Demo" == Version::OGAM_DEMO_STATUS)
	{
		demoStatus = Version::DEMO;
	}

	return demoStatus;
}
