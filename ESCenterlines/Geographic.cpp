#include "Geographic.h"



CGeographic::CGeographic() 
	: geodesic(GeographicLib::Geodesic::WGS84())
{
}


CGeographic::~CGeographic()
{
}

void CGeographic::CalculateExtendedCenterline(CRunway & runway, CCoordinate * coordinate)
{
	if (!runway.extended_centerline.course)
		CalculateApproachCourse(runway, coordinate);
	else
		runway.calculated_approach_course = runway.extended_centerline.course;
	CalculateCenterline(runway);
	CalculateRangeTicks(runway);
}

double CGeographic::GetAzimuth(const CCoordinate & c1, const CCoordinate & c2)
{
	double azi, azi2;
	geodesic.Inverse(c1.m_Latitude, c1.m_Longitude, c2.m_Latitude, c2.m_Longitude, azi, azi2);
	return azi;
}

CCoordinate CGeographic::GetCoordinate(CCoordinate c1, double azimuth, double distance)
{
	double lat2, lon2;
	geodesic.Direct(c1.m_Latitude, c1.m_Longitude, azimuth, distance, lat2, lon2);
	return CCoordinate(lat2, lon2);
}

double CGeographic::GetDistance(const CCoordinate & c1, const CCoordinate & c2)
{
	return 0.0;
}

void CGeographic::CalculateCenterline(CRunway & runway)
{
	auto pos = 0.0;
	for (auto & cl : runway.extended_centerline.centerline_elements)
	{
		auto pattern_length = cl.tick_length + cl.gap_length;
		for (auto i = 0; i < cl.length; ++i)
		{
			auto line_start = i * pattern_length + pos;
			if (cl.starts_with_gap)
				line_start += cl.gap_length;
			auto line_end = line_start + cl.tick_length;
			auto c1 = GetCoordinate(runway.threshold, runway.GetApproachCourse(), -line_start * GeographicLib::Constants::nauticalmile());
			auto c2 = GetCoordinate(runway.threshold, runway.GetApproachCourse(), -line_end * GeographicLib::Constants::nauticalmile());
			runway.AddLine(CLine(c1, c2));
		}
		pos += pattern_length * cl.length;
	}
}

void CGeographic::CalculateRangeTicks(CRunway & runway)
{
	for (auto & rt : runway.extended_centerline.range_ticks)
	{
		auto tick_azimuth_left = runway.GetApproachCourse() - 90;
		auto c_base = GetCoordinate(runway.threshold, runway.GetApproachCourse(), -rt.distance_thr * GeographicLib::Constants::nauticalmile());
		if (rt.direction == Direction::left || rt.direction == Direction::both)
		{
			auto c1_left = GetCoordinate(c_base, tick_azimuth_left, rt.distance_cl * GeographicLib::Constants::nauticalmile());
			auto c2_left = GetCoordinate(c1_left, tick_azimuth_left, rt.length * GeographicLib::Constants::nauticalmile());
			if (rt.depends)
				runway.AddLine(CLine(c1_left, c2_left, rt.depends_on.airport_designator, rt.depends_on.runway_designator));
			else
				runway.AddLine(CLine(c1_left, c2_left));
		}
		if (rt.direction == Direction::right || rt.direction == Direction::both)
		{
			auto c1_right = GetCoordinate(c_base, tick_azimuth_left, -rt.distance_cl * GeographicLib::Constants::nauticalmile());
			auto c2_right = GetCoordinate(c1_right, tick_azimuth_left, -rt.length * GeographicLib::Constants::nauticalmile());
			if (rt.depends)
				runway.AddLine(CLine(c1_right, c2_right, rt.depends_on.airport_designator, rt.depends_on.runway_designator));
			else
				runway.AddLine(CLine(c1_right, c2_right));
		}
	}
}

void CGeographic::CalculateApproachCourse(CRunway & runway, CCoordinate * coordinate)
{
	if (coordinate)
		runway.calculated_approach_course = GetAzimuth(*coordinate, runway.threshold);
	else
		runway.calculated_approach_course = GetAzimuth(runway.threshold, runway.stop_end);
}
