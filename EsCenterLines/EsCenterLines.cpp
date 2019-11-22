#include "EsCenterLines.h"

const char *plugin_name{ "EsCenterLines" };
const char *plugin_version{ "1.2" };
const char *plugin_author{ "Oliver Grützmann" };
const char *plugin_license{ "tbd" };

EsCenterLines::EsCenterLines()
	: EuroScopePlugIn::CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, plugin_name, plugin_version, plugin_author, plugin_license)
{
	ReadRunways();
	CalculateCenterlines();
	EuroScopePlugIn::CPlugIn::RegisterDisplayType("Center Lines", false, true, false, false);
}

EsCenterLines::~EsCenterLines()
{
}

EuroScopePlugIn::CRadarScreen *EsCenterLines::OnRadarScreenCreated(const char *sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated)
{
	if (GeoReferenced)
	{
		auto cls = new CenterLinesScreen(lines, ticks, screens);
		screens.push_back(cls);
		return cls;
	}
	return nullptr;
}

void EsCenterLines::OnAirportRunwayActivityChanged()
{
	ReadRunways();
	CalculateCenterlines();
	for (auto x : screens)
		x->RefreshMapContent();

}

void EsCenterLines::CalculateCenterlines()
{
	lines.clear();
	ticks.clear();
	for (auto x : runways)
	{
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
			gd_line.Position(dist + ll, lat2, lon2);
			lines.push_back({ { lat, lon }, { lat2, lon2 }, x.apt_active, x.rwy_active });
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
			ticks.push_back({ tick_left_start, tick_left_end, x.apt_active, x.rwy_active });
		}

		if (x.tick_direction == Direction::right || x.tick_direction == Direction::both)
		{
			auto tick_line = geo.Line(tick_origin.latitude, tick_origin.longitude, track - x.tick_angle);
			Coordinate tick_right_start, tick_right_end;
			tick_line.Position(dist_cl, tick_right_start.latitude, tick_right_start.longitude);
			tick_line.Position(dist_cl + length, tick_right_end.latitude, tick_right_end.longitude);
			ticks.push_back({ tick_right_start, tick_right_end, x.apt_active, x.rwy_active });
		}
	}
}

void EsCenterLines::CalculateMunichSpecial(const Runway_Definition &rd)
{
	auto line_track = rd.runway_heading_calculated - 180;
	auto gd_line = geo.Line(rd.runway_threshold.latitude, rd.runway_threshold.longitude, line_track);
	if (rd.runway_designator == "26R")
	{
		auto dist_thr = NauticalMiles(13.05);
		auto dist_thr2 = NauticalMiles(16.05);
		auto dist_thr3 = dist_thr + NauticalMiles(12);
		Coordinate origin, origin2, origin3, end, end2, end3;
		gd_line.Position(dist_thr, origin.latitude, origin.longitude);
		gd_line.Position(dist_thr2, origin2.latitude, origin2.longitude);
		gd_line.Position(dist_thr3, origin3.latitude, origin3.longitude);
		auto f_line = geo.Line(origin.latitude, origin.longitude, line_track - 90);
		auto f_line2 = geo.Line(origin2.latitude, origin2.longitude, line_track - 90);
		auto f_line3 = geo.Line(origin3.latitude, origin3.longitude, line_track - 90);
		f_line.Position(1.755 * GeographicLib::Constants::nauticalmile(), end.latitude, end.longitude);
		f_line2.Position(1.755 * GeographicLib::Constants::nauticalmile(), end2.latitude, end2.longitude);
		f_line3.Position(1.755 * GeographicLib::Constants::nauticalmile(), end3.latitude, end3.longitude);
		lines.push_back({ origin, end, rd.apt_active_arrival, rd.active_arrival });
		lines.push_back({ origin2, end2, rd.apt_active_arrival, rd.active_arrival });
		lines.push_back({ end, end3, rd.apt_active_arrival, rd.active_arrival });
	}
	else if (rd.runway_designator == "26L")
	{

	}
	else if (rd.runway_designator == "08R")
	{
		auto dist_thr = NauticalMiles(13.05);
		auto dist_thr2 = NauticalMiles(16.05);
		auto dist_thr3 = dist_thr + NauticalMiles(12);
		Coordinate origin, origin2, origin3, end, end2, end3;
		gd_line.Position(dist_thr, origin.latitude, origin.longitude);
		gd_line.Position(dist_thr2, origin2.latitude, origin2.longitude);
		gd_line.Position(dist_thr3, origin3.latitude, origin3.longitude);
		auto f_line = geo.Line(origin.latitude, origin.longitude, line_track - 90);
		auto f_line2 = geo.Line(origin2.latitude, origin2.longitude, line_track - 90);
		auto f_line3 = geo.Line(origin3.latitude, origin3.longitude, line_track - 90);
		f_line.Position(1.755 * GeographicLib::Constants::nauticalmile(), end.latitude, end.longitude);
		f_line2.Position(1.755 * GeographicLib::Constants::nauticalmile(), end2.latitude, end2.longitude);
		f_line3.Position(1.755 * GeographicLib::Constants::nauticalmile(), end3.latitude, end3.longitude);
		lines.push_back({ origin, end, rd.apt_active_arrival, rd.active_arrival });
		lines.push_back({ origin2, end2, rd.apt_active_arrival, rd.active_arrival });
		lines.push_back({ end, end3, rd.apt_active_arrival, rd.active_arrival });
	}
	else if (rd.runway_designator == "08L")
	{

	}
}

bool EsCenterLines::GetFixPosition(Coordinate &coord, const Runway_Definition &rwy)
{
	SectorContainer container(this, EuroScopePlugIn::SECTOR_ELEMENT_FIX);
	for (auto x : container)
	{
		if (!strcmp(x.GetName(), rwy.approach_fix.c_str()))
		{
			EuroScopePlugIn::CPosition position, thr_position;
			x.GetPosition(&position, 0);
			thr_position.m_Latitude = rwy.runway_threshold.latitude;
			thr_position.m_Longitude = rwy.runway_threshold.longitude;
			if (position.DistanceTo(thr_position) > 30)
				continue;
			coord.latitude = position.m_Latitude;
			coord.longitude = position.m_Longitude;
			return true;
		}
	}
	return false;
}

double EsCenterLines::NauticalMiles(double nm)
{
	return nm * GeographicLib::Constants::nauticalmile();
}

void EsCenterLines::ReadRunways()
{
	runways.clear();
	SectorContainer container(this, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
	for (auto runway : container)
	{
#ifdef __EDDM
		std::string name2 = runway.GetAirportName();
		if (!name2.starts_with("EDDM"))
			continue;
#endif
		bool apt_active_arrival{ false };
		SectorContainer container(this, EuroScopePlugIn::SECTOR_ELEMENT_AIRPORT);
		for (auto airport : container)
		{
			auto name = airport.GetName();
			if (!strncmp(name, runway.GetAirportName(), strlen(name)))
				apt_active_arrival = airport.IsElementActive(false);
		}
		for (int i = 0; i < 2; ++i)
		{

			Runway_Definition rwy;
			rwy.active_arrival = runway.IsElementActive(false, i);
			rwy.active_departure = runway.IsElementActive(true, i);
			rwy.apt_active_arrival = apt_active_arrival;
			rwy.airport_name = runway.GetAirportName();
			rwy.runway_designator = runway.GetRunwayName(i);
			if (rwy.airport_name.starts_with("EDDM"))
			{
				rwy.ticks.push_back(Tick_Definition(0.5, 90, 0, 12, Direction::both, rwy.active_arrival, apt_active_arrival));
				rwy.ticks.push_back(Tick_Definition(0.5, 90, 0, 15, Direction::both, rwy.active_arrival, apt_active_arrival));
				rwy.ticks.push_back(Tick_Definition(0.5, 90, 0, 22, Direction::both, rwy.active_arrival, apt_active_arrival));
				rwy.line_parts.push_back(Line_Definition(1, 1, 14, 0, false, rwy.active_arrival, apt_active_arrival));
				if (rwy.runway_designator == "08L")
				{
					rwy.approach_fix = "MAGAT";
					rwy.ticks.push_back({ 0.5,90,0.5,4,Direction::left, rwy.active_arrival, apt_active_arrival });
					rwy.ticks.push_back(Tick_Definition(0.3, 45, 0, 13.05, Direction::left, rwy.active_arrival, apt_active_arrival));
					rwy.ticks.push_back(Tick_Definition(0.3, 45, 0, 16.05, Direction::left, rwy.active_arrival, apt_active_arrival));
				}
				else if (rwy.runway_designator == "08R")
				{
					rwy.ticks.push_back({ 0.5,90,0.5,4,Direction::right, rwy.active_arrival, apt_active_arrival });
					rwy.ticks.push_back(Tick_Definition(0.5, 90, 0, 18, Direction::both, rwy.active_arrival, apt_active_arrival));
					rwy.ticks.push_back(Tick_Definition(0.3, 45, 0, 13.05, Direction::right, rwy.active_arrival, apt_active_arrival));
					rwy.ticks.push_back(Tick_Definition(0.3, 45, 0, 16.05, Direction::right, rwy.active_arrival, apt_active_arrival));
					rwy.approach_fix = "BEGEN";
				}
				else if (rwy.runway_designator == "26R")
				{
					rwy.ticks.push_back({ 0.5,90,0.5,4,Direction::right, rwy.active_arrival, apt_active_arrival });
					rwy.ticks.push_back(Tick_Definition(0.3, 45, 0, 13.05, Direction::right, rwy.active_arrival, apt_active_arrival));
					rwy.ticks.push_back(Tick_Definition(0.3, 45, 0, 16.05, Direction::right, rwy.active_arrival, apt_active_arrival));
					rwy.approach_fix = "GUDEG";
				}
				else if (rwy.runway_designator == "26L")
				{
					rwy.ticks.push_back({ 0.5,90,0.5,4,Direction::left, rwy.active_arrival, apt_active_arrival });
					rwy.ticks.push_back(Tick_Definition(0.3, 45, 0, 13.05, Direction::left, rwy.active_arrival, apt_active_arrival));
					rwy.ticks.push_back(Tick_Definition(0.3, 45, 0, 16.05, Direction::left, rwy.active_arrival, apt_active_arrival));
					rwy.approach_fix = "NELBI";
				}
			}
			rwy.runway_heading = runway.GetRunwayHeading(i);
			EuroScopePlugIn::CPosition thr, end;
			runway.GetPosition(&thr, i);
			runway.GetPosition(&end, i == 1 ? 0 : 1);
			rwy.runway_threshold = { thr.m_Latitude, thr.m_Longitude };
			rwy.runway_end = { end.m_Latitude, end.m_Longitude };
			Coordinate coord;
			if (rwy.approach_fix.length() != 0 && GetFixPosition(coord, rwy))
			{
				double azi1, azi2;
				geo.Inverse(coord.latitude, coord.longitude, rwy.runway_threshold.latitude, rwy.runway_threshold.longitude, azi1, azi2);
				rwy.runway_heading_calculated = azi2;
			}
			else
			{
				double azi1, azi2;
				geo.Inverse(rwy.runway_threshold.latitude, rwy.runway_threshold.longitude, rwy.runway_end.latitude, rwy.runway_end.longitude, azi1, azi2);
				rwy.runway_heading_calculated = (azi1 + azi2) / 2;
			}
			if (!rwy.ticks.size())
				rwy.ticks = default_ticks;
			for (auto &z : rwy.ticks)
			{
				z.apt_active = apt_active_arrival;
				z.rwy_active = rwy.active_arrival;
			}
			if (!rwy.line_parts.size())
				rwy.line_parts = default_line;
			for (auto &z : rwy.line_parts)
			{
				z.apt_active = apt_active_arrival;
				z.rwy_active = rwy.active_arrival;
			}
			runways.push_back(rwy);
		}
	}
}
