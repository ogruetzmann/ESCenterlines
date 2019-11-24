#pragma once
#include <list>
#include <string>
#include <algorithm>
#include <EuroScopePlugIn.h>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/GeodesicLine.hpp>
#include <GeographicLib/Math.hpp>
#include "Data.h"
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
	std::list<CenterLinesScreen *> screens;

	EuroScopePlugIn::CRadarScreen *OnRadarScreenCreated(const char *sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated);
	void OnAirportRunwayActivityChanged();
	void CalculateLine(Runway_Definition &rd, bool apt_act, bool rwy_act);
	void CalculateSpecial(Runway_Definition &rd, bool apt_act, bool rwy_act);
	void CalculateTicks(Runway_Definition &rd, bool apt_act, bool rwy_act);
	bool GetFixPosition(const std::string &fix, const EuroScopePlugIn::CPosition &threshold, Coordinate &coord);
	bool GetRunwaySettings(const std::string &airport, const std::string &runway, Runway_Settings &rws);

	bool IsAirportActive(const std::string &airport);
	inline double NauticalMiles(double nm) { return nm * GeographicLib::Constants::nauticalmile(); }
	void ReadRunways();
};


