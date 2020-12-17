#include "EsCenterLines.h"

#ifdef __EDDM
const char *plugin_name{ "EsCenterLines-EDDM" };
#else
const char *plugin_name{ "EsCenterLines" };
#endif
const char *plugin_version{ "1.32" };
const char *plugin_author{ "Oliver Grtzmann" };
const char *plugin_license{ "GPL v3" };

EsCenterLines::EsCenterLines()
	: EuroScopePlugIn::CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, plugin_name, plugin_version, plugin_author, plugin_license)
{
	ReadRunways();
	EuroScopePlugIn::CPlugIn::RegisterDisplayType("ES CenterLines", false, true, false, false);
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
	for (auto x : screens)
		x->RefreshMapContent();

}

void EsCenterLines::CalculateLine(Runway_Definition &rd, bool apt_act, bool rwy_act)
{
	double gl, ll, glll, dist;
	auto gd_line = geo.Line(rd.runway_threshold.m_Latitude, rd.runway_threshold.m_Longitude, rd.runway_heading_calculated - 180);
	for (auto &centerlines : rd.runway_settings.lines)
	{
		gl = NauticalMiles(centerlines.length_gap);
		ll = NauticalMiles(centerlines.length_line);
		glll = gl + ll;
		dist = centerlines.starts_with_line ? 0 : gl;
		dist += NauticalMiles(centerlines.distance_thr);
		for (auto i = 0; i < centerlines.repeats; ++i)
		{
			double lat, lon, lat2, lon2;
			gd_line.Position(dist, lat, lon);
			gd_line.Position(dist + ll, lat2, lon2);
			lines.push_back({ { lat, lon }, { lat2, lon2 }, apt_act, rwy_act });
			dist += glll;
		}
	}
}

void EsCenterLines::CalculateSpecial(Runway_Definition &rd, bool apt_act, bool rwy_act)
{
	if (!rd.runway_settings.special.length)
		return;
	auto gd_line = geo.Line(rd.runway_threshold.latitude, rd.runway_threshold.longitude, rd.runway_heading_calculated - 180);
	auto dist_thr = NauticalMiles(rd.runway_settings.special.distance_thr1);
	auto dist_thr2 = NauticalMiles(rd.runway_settings.special.distance_thr2);
	auto dist_thr3 = NauticalMiles(rd.runway_settings.special.distance_thr1 + rd.runway_settings.special.length);
	Coordinate origin, origin2, origin3, end, end2, end3;
	gd_line.Position(dist_thr, origin.latitude, origin.longitude);
	gd_line.Position(dist_thr2, origin2.latitude, origin2.longitude);
	gd_line.Position(dist_thr3, origin3.latitude, origin3.longitude);
	auto line_direction = (rd.runway_settings.special.direction == Direction::left ? rd.runway_heading_calculated - 180 + 90 : rd.runway_heading_calculated - 180 - 90);
	auto f_line = geo.Line(origin.latitude, origin.longitude, line_direction);
	auto f_line2 = geo.Line(origin2.latitude, origin2.longitude, line_direction);
	auto f_line3 = geo.Line(origin3.latitude, origin3.longitude, line_direction);
	f_line.Position(NauticalMiles(rd.runway_settings.special.distance_cl), end.latitude, end.longitude);
	f_line2.Position(NauticalMiles(rd.runway_settings.special.distance_cl), end2.latitude, end2.longitude);
	f_line3.Position(NauticalMiles(rd.runway_settings.special.distance_cl), end3.latitude, end3.longitude);
	lines.push_back({ origin, end, apt_act, rwy_act });
	lines.push_back({ origin2, end2, apt_act, rwy_act });
	lines.push_back({ end, end3, apt_act, rwy_act });
}

void EsCenterLines::CalculateTicks(Runway_Definition &rd, bool apt_act, bool rwy_act)
{
	double dist_thr, dist_cl, length;
	auto gd_line = geo.Line(rd.runway_threshold.latitude, rd.runway_threshold.longitude, rd.runway_heading_calculated - 180);
	for (auto &rangeticks : rd.runway_settings.ticks)
	{
		dist_thr = NauticalMiles(rangeticks.distance_thr);
		dist_cl = NauticalMiles(rangeticks.distance_cl);
		length = NauticalMiles(rangeticks.length);
		Coordinate tick_origin;
		gd_line.Position(dist_thr, tick_origin.latitude, tick_origin.longitude);

		if (rangeticks.direction == Direction::left || rangeticks.direction == Direction::both)
		{
			auto tick_line = geo.Line(tick_origin.latitude, tick_origin.longitude, rd.runway_heading_calculated - 180 + rangeticks.angle);
			Coordinate tick_left_start, tick_left_end;
			tick_line.Position(dist_cl, tick_left_start.latitude, tick_left_start.longitude);
			tick_line.Position(dist_cl + length, tick_left_end.latitude, tick_left_end.longitude);
			ticks.push_back({ tick_left_start, tick_left_end, apt_act, rwy_act });
		}

		if (rangeticks.direction == Direction::right || rangeticks.direction == Direction::both)
		{
			auto tick_line = geo.Line(tick_origin.latitude, tick_origin.longitude, rd.runway_heading_calculated - 180 - rangeticks.angle);
			Coordinate tick_right_start, tick_right_end;
			tick_line.Position(dist_cl, tick_right_start.latitude, tick_right_start.longitude);
			tick_line.Position(dist_cl + length, tick_right_end.latitude, tick_right_end.longitude);
			ticks.push_back({ tick_right_start, tick_right_end, apt_act, rwy_act });
		}
	}
}

bool EsCenterLines::GetFixPosition(const std::string &fix, const EuroScopePlugIn::CPosition &threshold, Coordinate &coord)
{
	SelectActiveSectorfile();
	for (auto element = SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_FIX); 
		element.IsValid(); 
		element = SectorFileElementSelectNext(element, EuroScopePlugIn::SECTOR_ELEMENT_FIX))
	{ 
		if (!strcmp(element.GetName(), fix.c_str()))
		{
			element.GetPosition(&coord, 0);
			if (coord.DistanceTo(threshold) > 30)
				continue;
			return true;
		}
	}
	return false;
}

bool EsCenterLines::GetRunwaySettings(const std::string &airport, const std::string &runway, Runway_Settings &rws)
{
	Data set;
	for (auto apt : set.apts)
	{
		if (airport.starts_with(apt.airport))
		{
			for (auto rwy : apt.runways)
			{
				if (rwy.runway == runway)
				{
					if (rwy.lines.size() == 0)
						rwy.lines = apt.lines;
					if (rwy.ticks.size() == 0)
						rwy.ticks = apt.ticks;
					rws = rwy;
					return true;
				}
			}
		}
	}
	return false;
}

bool EsCenterLines::IsAirportActive(const std::string &airport)
{
	SectorContainer container(this, EuroScopePlugIn::SECTOR_ELEMENT_AIRPORT);
	for (auto apt : container)
	{
		std::string name = apt.GetName();
		if (apt.IsElementActive(false) && airport.starts_with(name))
			return true;
	}
	return false;
}

void EsCenterLines::ReadRunways()
{
	lines.clear();
	ticks.clear();
	SectorContainer container(this, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
	for (auto runway = SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY); 
		runway.IsValid(); 
		runway = SectorFileElementSelectNext(runway, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
	{
		for (int i = 0; i < 2; ++i)
		{
			Runway_Definition rwy;
			rwy.runway_designator = runway.GetRunwayName(i);
			rwy.airport_name = runway.GetAirportName();
			rwy.active_arrival = runway.IsElementActive(false, i);
			rwy.apt_active_arrival = IsAirportActive(rwy.airport_name);
			
			if (!GetRunwaySettings(rwy.airport_name, rwy.runway_designator, rwy.runway_settings))
			{
				rwy.runway_settings.lines = zzzz.get().lines;
				rwy.runway_settings.ticks = zzzz.get().ticks;
			}
			Coordinate coord;
			EuroScopePlugIn::CPosition threshold, end;
			runway.GetPosition(&threshold, i);
			runway.GetPosition(&end, i == 1 ? 0 : 1);
			rwy.runway_threshold.latitude = threshold.m_Latitude;
			rwy.runway_threshold.longitude = threshold.m_Longitude;
			rwy.runway_end.latitude = end.m_Latitude;
			rwy.runway_end.longitude = end.m_Longitude;
			if (rwy.runway_settings.fix.size() && GetFixPosition(rwy.runway_settings.fix, threshold, coord))
			{
				double azi1, azi2;
				geo.Inverse(coord.latitude, coord.longitude, rwy.runway_threshold.latitude, rwy.runway_threshold.longitude, azi1, azi2);
				rwy.runway_heading_calculated = azi2;
			}
			else
			{
				double azi1, azi2;
				geo.Inverse(rwy.runway_threshold.latitude, rwy.runway_threshold.longitude, rwy.runway_end.latitude, rwy.runway_end.longitude, azi1, azi2);
				rwy.runway_heading_calculated = azi2;
			}
			CalculateLine(rwy, rwy.apt_active_arrival, rwy.active_arrival);
			CalculateTicks(rwy, rwy.apt_active_arrival, rwy.active_arrival);
			CalculateSpecial(rwy, rwy.apt_active_arrival, rwy.active_arrival);
		}
	}
}
