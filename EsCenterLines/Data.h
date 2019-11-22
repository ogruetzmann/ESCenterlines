#pragma once
#include <string>
#include <list>
#include "Coordinate.h"

struct Line_Definition
{
	Line_Definition(double line_length, double gap_length, int repeats, double distance_thr, bool starts_with_line, bool rwy_active, bool apt_active)
		: line_length(line_length), gap_length(gap_length), length(repeats), distance_from_threshold(distance_thr), 
		starts_with_line(starts_with_line), rwy_active(rwy_active), apt_active(apt_active) {}
	double line_length{ 1 };
	double gap_length{ 1 };
	int length{ 10 };
	double distance_from_threshold{ 0 };
	bool starts_with_line{ false };
	bool rwy_active{ false };
	bool apt_active{ false };
};

enum class Direction { both = 0, left, right };

struct Tick_Definition
{
	Tick_Definition(double length, double angle, double distance_cl, double distance_thr, Direction direction, bool rwy_active, bool apt_active)
		: tick_length(length), tick_angle(angle), tick_distance_from_centerline(distance_cl), tick_distance_from_threshold(distance_thr), 
		tick_direction(direction), rwy_active(rwy_active), apt_active(apt_active) {}
	double tick_length{ 0.5 };
	double tick_angle{ 90 };
	double tick_distance_from_centerline{ 0.5 };
	double tick_distance_from_threshold{ 4 };
	Direction tick_direction{ Direction::both };
	bool rwy_active{ false };
	bool apt_active{ false };
};

struct Runway_Definition
{
	std::string runway_designator{};
	std::string airport_name{};
	Coordinate runway_threshold{ 0,0 };
	Coordinate runway_end{ 0,0 };
	bool active_arrival{ false };
	bool active_departure{ false };
	bool apt_active_arrival{ false };
	bool apt_active_departure{ false };
	double runway_heading{ 0.0 };
	double runway_heading_calculated{ 0.0 };
	std::string approach_fix{};
	std::list<Tick_Definition> ticks;
	std::list<Line_Definition> line_parts;
};

struct Tick_Settings
{
	double length;
	double angle;
	double distance_cl;
	double distance_thr;
	Direction direction;
};

struct Line_Settings
{
	double length_line;
	double length_gap;
	int repeats;
	double distance_thr;
	bool starts_with_line;
};

struct Runway_Settings
{
	std::string airport;
	std::string runway;
	std::list<Line_Settings> lines;
	std::list<Tick_Settings> ticks;
};

static std::list<Tick_Definition> default_ticks{ {0.5, 90, 0.5, 4, Direction::both, false, false}, {0.5, 90, 0, 10, Direction::both, false, false}, {0.5, 90, 0, 20, Direction::both, false, false} };
static std::list<Line_Definition> default_line{ {1, 1, 10, 0, false, false, false} };
