#include "Geographic.h"

CGeographic::CGeographic()
	: geodesic(GeographicLib::Geodesic::WGS84())
{
}


CGeographic::~CGeographic()
{
}

void CGeographic::CalculateExtendedCenterline(const CRunway& runway, CExtendedCenterline& cl, const CCoordinate* coordinate, std::vector<CLine>& l)
{
	double course { 0 };
	if (!(course = cl.GetCourse()))
		course = cl.SetCourse(CalculateApproachCourse(runway, coordinate));
	CalculateCenterline(runway, cl, l, course);
	CalculateRangeTicks(runway, cl, l, course);
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

double CGeographic::GetDistance(const CCoordinate& c1, const CCoordinate & c2)
{
	return 0.0;
}

void CGeographic::CalculateCenterline(const CRunway& runway, const CExtendedCenterline& centerline, std::vector<CLine>& l, double course)
{
	auto pos = 0.0;
	for (auto & cl : centerline.GetElements())
	{
		auto pattern_length = cl.dash_length + cl.gap_length;
		for (auto i = 0; i < cl.number; ++i)
		{
			auto line_start = i * pattern_length + pos;
			if (cl.starts_gap)
				line_start += cl.gap_length;
			auto line_end = line_start + cl.dash_length;
			auto c1 = GetCoordinate(runway.GetThresholdPosition(), course, line_start * GeographicLib::Constants::nauticalmile());
			auto c2 = GetCoordinate(runway.GetThresholdPosition(), course, line_end * GeographicLib::Constants::nauticalmile());
			l.push_back(CLine(runway.GetId(), c1, c2));
		}
		pos += pattern_length * cl.number;
	}
}

void CGeographic::CalculateRangeTicks(const CRunway& runway, const CExtendedCenterline& centerline, std::vector<CLine>& l, double course)
{
	for (auto & rt : centerline.GetMarkers())
	{
		auto tick_azimuth_left = course - 90;
		auto c_base = GetCoordinate(runway.GetThresholdPosition(), course, rt.dist_thr * GeographicLib::Constants::nauticalmile());
		if (rt.direction == Direction::left || rt.direction == Direction::both)
		{
			auto c1_left = GetCoordinate(c_base, tick_azimuth_left, rt.dist_cl * GeographicLib::Constants::nauticalmile());
			auto c2_left = GetCoordinate(c1_left, tick_azimuth_left, rt.length * GeographicLib::Constants::nauticalmile());
			if (rt.depends_on)
				l.push_back(CLine(runway.GetId(), c1_left, c2_left, *rt.depends_on));
			else
				l.push_back(CLine(runway.GetId(), c1_left, c2_left));
		}
		if (rt.direction == Direction::right || rt.direction == Direction::both)
		{
			auto c1_right = GetCoordinate(c_base, tick_azimuth_left, -rt.dist_cl * GeographicLib::Constants::nauticalmile());
			auto c2_right = GetCoordinate(c1_right, tick_azimuth_left, -rt.length * GeographicLib::Constants::nauticalmile());
			if (rt.depends_on)
				l.push_back(CLine(runway.GetId(), c1_right, c2_right, *rt.depends_on));
			else
				l.push_back(CLine(runway.GetId(), c1_right, c2_right));
		}
	}
}

double CGeographic::CalculateApproachCourse(const CRunway& runway, const CCoordinate* coordinate)
{
	if (coordinate)
		return GetAzimuth(runway.GetThresholdPosition(), *coordinate);
	else
		return GetAzimuth(runway.GetThresholdPosition(), runway.GetStopEndPosition()) + 180;
}
