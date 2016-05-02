#pragma once
#include <Windows.h>
#include "EuroScopePlugIn.h"
#include "ESCenterlinesScreen.h"

using namespace EuroScopePlugIn;

#define MY_PLUGIN_NAME      "ESCenterlines"
#define MY_PLUGIN_VERSION   "0.1"
#define MY_PLUGIN_DEVELOPER "Oliver Grützmann"
#define MY_PLUGIN_COPYRIGHT "MIT license"

class CESCenterlines :
	public EuroScopePlugIn::CPlugIn
{
private:
	FILETIME ActiveRunwaysUpdateTime;

public:
	CESCenterlines();
	virtual ~CESCenterlines();

	EuroScopePlugIn::CRadarScreen * OnRadarScreenCreated(const char * sDisplayName,
														 bool NeedRadarContent,
														 bool GeoReferenced,
														 bool CanBeSaved,
														 bool CanBeCreated);
	void OnAirportRunwayActivityChanged();
};
