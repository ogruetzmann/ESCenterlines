#include "ExtendedCenterline.h"



CExtendedCenterline::CExtendedCenterline()
{
}

CExtendedCenterline::~CExtendedCenterline()
{
}

void CExtendedCenterline::AddElement(CCenterlineElement & element)
{
	centerline_elements.push_back(element);
}

void CExtendedCenterline::AddElement(double tick_length, double gap_length, double length, bool starts_with_gap)
{
	centerline_elements.push_back(CCenterlineElement(tick_length, gap_length, length, starts_with_gap));
}

void CExtendedCenterline::AddRangeTick(CRangeTick & rt)
{
	range_ticks.push_back(rt);
}

void CExtendedCenterline::AddRangeTick(double distance_from_threshold, double distance_from_centerline, double length, Direction direction, double angle)
{
	range_ticks.push_back(CRangeTick(distance_from_threshold, distance_from_centerline, length, direction, angle));
}

void CExtendedCenterline::AddRangeTick(double distance_from_threshold, 
									   double distance_from_centerline, 
									   double length, 
									   std::string depends_airport, 
									   std::string depends_runway, 
									   Direction direction, 
									   double angle)
{
	range_ticks.push_back(CRangeTick(distance_from_threshold, distance_from_centerline, length, Identifier(depends_airport, depends_runway), direction, angle));
}

void CExtendedCenterline::SetDefault()
{
	AddRangeTick(4, 0.5, 0.5, Direction::both);
	AddRangeTick(10, 0.5, 0.5, Direction::both);
	AddRangeTick(20, 0.5, 0.5, Direction::both);
	AddElement(1, 1, 10, true);
	AddElement(4, 1, 2, true);
}

