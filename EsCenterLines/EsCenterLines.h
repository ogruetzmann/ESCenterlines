#pragma once
#include <list>
#include <string>
#include <string_view>
#include <vector>
#include <EuroScopePlugIn.h>
#include "Runway.h"
class EsCenterLines :
	public EuroScopePlugIn::CPlugIn
{
public:
	EsCenterLines();
	virtual ~EsCenterLines();

private:
	void OnAirportRunwayActivityChanged();
	EuroScopePlugIn::CRadarScreen *OnRadarScreenCreated(const char *sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated);
	void ReadRunways();

};

