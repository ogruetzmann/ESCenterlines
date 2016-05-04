#pragma once
#include <Windows.h>
#include <EuroScopePlugIn.h>
#include <memory>
#include "ESCenterlinesScreen.h"

using namespace EuroScopePlugIn;

const char MY_PLUGIN_NAME[] = "ESCenterlines";
const char MY_PLUGIN_VERSION[] = "0.2";
const char MY_PLUGIN_DEVELOPER[] = "Oliver Grützmann";
const char MY_PLUGIN_COPYRIGHT[] = "MIT license";

class CESCenterlines :
	public EuroScopePlugIn::CPlugIn
{
private:
	FILETIME ActiveRunwaysUpdateTime;
	std::unique_ptr<CCenterlineSettings> centerline_settings { std::make_unique<CCenterlineSettings>() };

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
