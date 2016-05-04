#include "ESCenterlines.h"

CESCenterlines::CESCenterlines()
	: CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
			  MY_PLUGIN_NAME,
			  MY_PLUGIN_VERSION,
			  MY_PLUGIN_DEVELOPER,
			  MY_PLUGIN_COPYRIGHT)
{
	RegisterDisplayType("ES Centerlines", false, true, false, false);
	GetSystemTimeAsFileTime(&ActiveRunwaysUpdateTime);
}


CESCenterlines::~CESCenterlines()
{
}

EuroScopePlugIn::CRadarScreen * CESCenterlines::OnRadarScreenCreated(const char * sDisplayName, 
																	 bool NeedRadarContent, 
																	 bool GeoReferenced, 
																	 bool CanBeSaved, 
																	 bool CanBeCreated)
{
	return new CESCenterlinesScreen(ActiveRunwaysUpdateTime, *centerline_settings);
}

void CESCenterlines::OnAirportRunwayActivityChanged()
{
	GetSystemTimeAsFileTime(&ActiveRunwaysUpdateTime);
}

