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
	std::list<Runway_Definition> runways;
	std::list<CLine> lines;
	std::list<CLine> ticks;

	EuroScopePlugIn::CRadarScreen *OnRadarScreenCreated(const char *sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated);
	void OnAirportRunwayActivityChanged();
	void CalculateCenterlines();
	void CalculateLine(std::list<Line_Definition> &ld, Coordinate &threshold, double track);
	void CalculateTicks(std::list<Tick_Definition> &td, Coordinate &threshold, double track);
	void ReadRunways();

};

