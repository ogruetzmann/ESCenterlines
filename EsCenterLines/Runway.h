#pragma once
#include <string>
#include <list>
#include "Coordinate.h"

struct Line_Definition
{
	double line_length{ 1 };
	double gap_length{ 1 };
	int length{ 10 };
	double distance_from_threshold{ 0 };
	bool starts_with_line{ false };
};

struct Tick_Definition
{
	double tick_length{ 0.5 };
	double tick_angle{ 90 };
	double tick_distance_from_centerline{ 0.5 };
	double tick_distance_from_threshold{ 4 };
};

struct Runway_Definition
{
	std::string runway_designator{};
	std::string airport_name{};
	Coordinate runway_threshold{ 0,0 };
	Coordinate runway_end{ 0,0 };
	bool active_arrival{ false };
	bool active_departure{ false };
	double runway_heading{ 0.0 };
	double runway_heading_calculated{ 0.0 };
	std::string approach_fix{};
	std::list<Tick_Definition> ticks{ { Tick_Definition()}, {0.5,90,0.5,10 }, {0.5,90,0.5,20} };
	std::list<Line_Definition> line_parts{ Line_Definition() };
};