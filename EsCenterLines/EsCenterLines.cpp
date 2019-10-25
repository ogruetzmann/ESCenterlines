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

EuroScopePlugIn::CRadarScreen *EsCenterLines::OnRadarScreenCreated(const char *sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated)
{
	if (GeoReferenced)
		return new CenterLinesScreen(lines, ticks);
	return nullptr;
}

void EsCenterLines::OnAirportRunwayActivityChanged()
{
	ReadRunways();
	CalculateCenterlines();
}

void EsCenterLines::CalculateCenterlines()
{
	lines.clear();
	ticks.clear();
	for (auto x : runways)
	{
		if (!x.active_arrival)
			continue;
		auto track = x.runway_heading_calculated - 180;
		CalculateLine(x.line_parts, x.runway_threshold, track);
		CalculateTicks(x.ticks, x.runway_threshold, track);
	}
}

void EsCenterLines::CalculateLine(std::list<Line_Definition> &ld, Coordinate &threshold, double track)
{
	for (auto x : ld)
	{
		auto gl = x.gap_length * GeographicLib::Constants::nauticalmile();
		auto ll = x.line_length * GeographicLib::Constants::nauticalmile();
		auto glll = gl + ll;
		auto dist = x.starts_with_line ? 0 : gl;
		dist += x.distance_from_threshold * GeographicLib::Constants::nauticalmile();
		auto gd_line = geo.Line(threshold.latitude, threshold.longitude, track);
		for (auto i = 0; i < x.length; ++i)
		{
			double lat, lon, lat2, lon2;
			gd_line.Position(dist, lat, lon);
			gd_line.Position(dist+ll, lat2, lon2);
			lines.push_back({ { lat, lon }, { lat2, lon2 } });
			dist += glll;
		}
	}
}

void EsCenterLines::CalculateTicks(std::list<Tick_Definition> &td, Coordinate &threshold, double track)
{
	auto gd_line = geo.Line(threshold.latitude, threshold.longitude, track);
	for (auto x : td)
	{
		auto dist = x.tick_distance_from_threshold * GeographicLib::Constants::nauticalmile();
		double lat, lon, lat2, lon2, lat3, lon3;
		gd_line.Position(dist, lat3, lon3);
		dist += x.tick_distance_from_centerline * GeographicLib::Constants::nauticalmile();
		gd_line.Position(dist, lat, lon);
		gd_line.Position(dist + x.tick_length * GeographicLib::Constants::nauticalmile(), lat2, lon2);
		ticks.push_back({ { lat, lon }, { lat2, lon2 }, {lat3, lon3} });
	}
}

void EsCenterLines::ReadRunways()
{
	runways.clear();
	GeographicLib::GeodesicLine line(geo, 0, 0, 0, GeographicLib::GeodesicLine::AZIMUTH);
	SectorContainer container(this, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
	for (auto x : container)
	{
		Runway_Definition rwy;
		for (int i = 0; i < 2; ++i)
		{
			rwy.airport_name = x.GetAirportName();
			rwy.runway_designator = x.GetRunwayName(i);
			rwy.runway_heading = x.GetRunwayHeading(i);
			EuroScopePlugIn::CPosition thr, end;
			x.GetPosition(&thr, i);
			x.GetPosition(&end, i == 1 ? 0 : 1);
			rwy.runway_threshold = { thr.m_Latitude, thr.m_Longitude };
			rwy.runway_end = { end.m_Latitude, end.m_Longitude };
			rwy.active_arrival = x.IsElementActive(false, i);
			rwy.active_departure = x.IsElementActive(true, i);
			line = geo.InverseLine(rwy.runway_threshold.latitude,
				rwy.runway_threshold.longitude,
				rwy.runway_end.latitude,
				rwy.runway_end.longitude,
				GeographicLib::Geodesic::AZIMUTH);
			rwy.runway_heading_calculated = line.Azimuth();
			rwy.line_parts.push_back({ 4,1,2,20,false });
			runways.push_back(rwy);
		}
	}
}

