#include "EsCenterLines.h"

const char *plugin_name{ "EsCenterLines" };
const char *plugin_version{ "0.0" };
const char *plugin_author{ "Oliver Grützmann" };
const char *plugin_license{ "tbd" };

EsCenterLines::EsCenterLines()
	: EuroScopePlugIn::CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, plugin_name, plugin_version, plugin_author, plugin_license)
{
	ReadRunways();
	EuroScopePlugIn::CPlugIn::RegisterDisplayType("Center Lines", false, true, false, false);
}

EsCenterLines::~EsCenterLines()
{
}

void EsCenterLines::OnAirportRunwayActivityChanged()
{
	ReadRunways();
}

EuroScopePlugIn::CRadarScreen *EsCenterLines::OnRadarScreenCreated(const char *sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated)
{
	if (GeoReferenced)
		return new CenterLinesScreen(runways);
	return nullptr;
}

void EsCenterLines::ReadRunways()
{
	runways.clear();
	GeographicLib::GeodesicLine line(geo, 0,0,0, GeographicLib::GeodesicLine::AZIMUTH);
	SectorContainer container(this, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
	for (auto x : container)
	{
		Runway rwy;
		EuroScopePlugIn::CPosition pos;
		for (int i = 0; i < 2; ++i)
		{
			rwy.airport_name = x.GetAirportName();
			rwy.runway_designator = x.GetRunwayName(i);
			rwy.runway_heading = x.GetRunwayHeading(i);
			x.GetPosition(&pos, i);
			rwy.runway_threshold = { pos.m_Latitude, pos.m_Longitude };
			x.GetPosition(&pos, i==1?0:1);
			rwy.runway_end = { pos.m_Latitude, pos.m_Longitude };
			rwy.active_arrival = x.IsElementActive(false, i);
			rwy.active_departure = x.IsElementActive(true, i);
			line = geo.InverseLine(rwy.runway_threshold.latitude, 
				rwy.runway_threshold.longitude,
				rwy.runway_end.latitude, 
				rwy.runway_end.longitude, 
				GeographicLib::GeodesicLine::AZIMUTH);
			rwy.runway_heading_calculated = line.Azimuth();
			auto line2 = geo.Line(rwy.runway_threshold.latitude, rwy.runway_threshold.longitude, rwy.runway_heading_calculated - 180);
			line2.Position(20000, rwy.endpoint.latitude, rwy.endpoint.longitude);
			runways.push_back(rwy);
		}
	}
}

