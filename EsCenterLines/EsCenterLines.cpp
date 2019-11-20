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
	{
		ReadRunways();
		CalculateCenterlines();
		return new CenterLinesScreen(lines, ticks);
	}
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
		if (x.airport_name.starts_with("EDDM"))
			CalculateMunichSpecial(x);
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
		auto dist_thr = x.tick_distance_from_threshold * GeographicLib::Constants::nauticalmile();
		Coordinate tick_origin;
		gd_line.Position(dist_thr, tick_origin.latitude, tick_origin.longitude);
		
		auto dist_cl = x.tick_distance_from_centerline * GeographicLib::Constants::nauticalmile();
		auto length = x.tick_length * GeographicLib::Constants::nauticalmile();
		
		if (x.tick_direction == Direction::left || x.tick_direction == Direction::both)
		{
			auto tick_line = geo.Line(tick_origin.latitude, tick_origin.longitude, track + x.tick_angle);
			Coordinate tick_left_start, tick_left_end;
			tick_line.Position(dist_cl, tick_left_start.latitude, tick_left_start.longitude);
			tick_line.Position(dist_cl + length, tick_left_end.latitude, tick_left_end.longitude);
			ticks.push_back({ tick_left_start, tick_left_end });
		}

		if (x.tick_direction == Direction::right || x.tick_direction == Direction::both)
		{
			auto tick_line = geo.Line(tick_origin.latitude, tick_origin.longitude, track - x.tick_angle);
			Coordinate tick_right_start, tick_right_end;
			tick_line.Position(dist_cl, tick_right_start.latitude, tick_right_start.longitude);
			tick_line.Position(dist_cl + length, tick_right_end.latitude, tick_right_end.longitude);
			ticks.push_back({ tick_right_start, tick_right_end });
		}

		//ticks.push_back({ { lat, lon }, { lat2, lon2 }, {lat3, lon3}, x.tick_angle, char(x.tick_direction) });
	}
}

void EsCenterLines::CalculateMunichSpecial(const Runway_Definition &rd)
{
	auto line_track = rd.runway_heading_calculated - 180;
	auto gd_line = geo.Line(rd.runway_threshold.latitude, rd.runway_threshold.longitude, line_track);
	if (rd.runway_designator == "26R")
	{
		auto dist_thr = 13.05 * GeographicLib::Constants::nauticalmile();
		auto dist_thr2 = 16.05 * GeographicLib::Constants::nauticalmile();
		Coordinate origin, origin2, end, end2, end3;
		gd_line.Position(dist_thr, origin.latitude, origin.longitude);
		gd_line.Position(dist_thr2, origin2.latitude, origin2.longitude);
		auto f_line = geo.Line(origin.latitude, origin.longitude, line_track - 90);
		auto f_line2 = geo.Line(origin2.latitude, origin2.longitude, line_track - 90);
		f_line.Position(1.755 * GeographicLib::Constants::nauticalmile(), end.latitude, end.longitude);
		f_line2.Position(1.755 * GeographicLib::Constants::nauticalmile(), end2.latitude, end2.longitude);
		auto f_line3 = geo.Line(end.latitude, end.longitude, line_track);
		f_line3.Position(12 * GeographicLib::Constants::nauticalmile(), end3.latitude, end3.longitude);
		lines.push_back({ origin, end });
		lines.push_back({ origin2, end2 });
		lines.push_back({ end, end3 });
	}
	else if (rd.runway_designator == "26L")
	{
		
	}
	else if (rd.runway_designator == "08R")
	{
		auto dist_thr = 13.05 * GeographicLib::Constants::nauticalmile();
		auto dist_thr2 = 16.05 * GeographicLib::Constants::nauticalmile();
		Coordinate origin, origin2, end, end2, end3;
		gd_line.Position(dist_thr, origin.latitude, origin.longitude);
		gd_line.Position(dist_thr2, origin2.latitude, origin2.longitude);
		auto f_line = geo.Line(origin.latitude, origin.longitude, line_track - 90);
		auto f_line2 = geo.Line(origin2.latitude, origin2.longitude, line_track - 90);
		f_line.Position(1.755 * GeographicLib::Constants::nauticalmile(), end.latitude, end.longitude);
		f_line2.Position(1.755 * GeographicLib::Constants::nauticalmile(), end2.latitude, end2.longitude);
		auto f_line3 = geo.Line(end.latitude, end.longitude, line_track);
		f_line3.Position(12 * GeographicLib::Constants::nauticalmile(), end3.latitude, end3.longitude);
		lines.push_back({ origin, end });
		lines.push_back({ origin2, end2 });
		lines.push_back({ end, end3 });
	}
	else if (rd.runway_designator == "08L")
	{

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
			rwy.ticks.push_back({ 0.5,90,0.5,4,Direction::right });
			rwy.ticks.push_back({ 0.5,90,0,12 });
			rwy.ticks.push_back({ 0.5,90,0,15 });
			rwy.ticks.push_back({ 0.5,90,0,22 });
			rwy.ticks.push_back({ 0.3,45,0,13.05,Direction::right });
			rwy.ticks.push_back({ 0.3,45,0,16.05,Direction::right });
			rwy.line_parts.push_back({ 1,1,14,0,false });
			runways.push_back(rwy);
		}
	}
}

