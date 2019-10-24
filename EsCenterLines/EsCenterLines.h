#pragma once
#include <list>
#include <string>
#include <vector>
#include <EuroScopePlugIn.h>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/GeodesicLine.hpp>
#include <GeographicLib/Math.hpp>
#include "Runway.h"
#include "EsHelpers.h"
#include "CenterLinesScreen.h"
class EsCenterLines :
	public EuroScopePlugIn::CPlugIn
{
	GeographicLib::Geodesic geo = GeographicLib::Geodesic::WGS84();
public:
	EsCenterLines();
	virtual ~EsCenterLines();

private:
	std::list<Runway> runways;

	void OnAirportRunwayActivityChanged();
	EuroScopePlugIn::CRadarScreen *OnRadarScreenCreated(const char *sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated);
	void ReadRunways();

};

