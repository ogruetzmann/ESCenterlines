#pragma once
#include <string>
#include <list>
#include "Coordinate.h"

enum class Direction { both = 0, left, right };

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

struct Special_Settings
{
	double distance_cl{ 0 };
	double distance_thr1{ 0 };
	double distance_thr2{ 0 };
	double length{ 0 };
	Direction direction{ 0 };
};

struct Runway_Settings
{
	std::string runway;
	std::string fix;
	std::list<Line_Settings> lines;
	std::list<Tick_Settings> ticks;
	Special_Settings special;
};

struct Airport_Settings
{
	std::string airport;
	std::list<Line_Settings> lines;
	std::list<Tick_Settings> ticks;
	std::list<Runway_Settings> runways;
};

struct Line_Definition
{
	Line_Definition() {}
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

struct Tick_Definition
{
	Tick_Definition() {}
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
	Runway_Settings runway_settings;
};

static std::list<Tick_Definition> default_ticks{ {0.5, 90, 0.5, 4, Direction::both, false, false}, {0.5, 90, 0, 10, Direction::both, false, false}, {0.5, 90, 0, 20, Direction::both, false, false} };
static std::list<Line_Definition> default_line{ {1, 1, 10, 0, false, false, false} };

struct EDDM
{
	Airport_Settings get()
	{
		Airport_Settings eddm;
		eddm.airport = "EDDM";

		Line_Settings eddm_lines;
		eddm_lines.distance_thr = 0;
		eddm_lines.length_gap = 1;
		eddm_lines.length_line = 1;
		eddm_lines.repeats = 14;
		eddm_lines.starts_with_line = false;
		eddm.lines.push_back(eddm_lines);

		Special_Settings spc_26r;
		spc_26r.distance_cl = 1.755;
		spc_26r.distance_thr1 = 13.05;
		spc_26r.distance_thr2 = 16.05;
		spc_26r.length = 12;
		spc_26r.direction = Direction::right;

		Runway_Settings eddm_26r;
		eddm_26r.fix = "GUDEG";
		eddm_26r.runway = "26R";
		eddm_26r.ticks.push_back({ 0.5, 90, 0.5, 4, Direction::right });
		eddm_26r.ticks.push_back({ 0.5, 90, 0, 12, Direction::both });
		eddm_26r.ticks.push_back({ 0.5, 90, 0, 15, Direction::both });
		eddm_26r.ticks.push_back({ 0.5, 90, 0, 22, Direction::both });
		eddm_26r.ticks.push_back({ 0.3, 45, 0, 13.05, Direction::right });
		eddm_26r.ticks.push_back({ 0.3, 45, 0, 16.05, Direction::right });
		eddm_26r.special = spc_26r;
		eddm.runways.push_back(eddm_26r);

		Runway_Settings eddm_26l;
		eddm_26l.fix = "NELBI";
		eddm_26l.runway = "26L";
		eddm_26l.ticks.push_back({ 0.5, 90, 0.5, 4, Direction::left });
		eddm_26l.ticks.push_back({ 0.5, 90, 0, 12, Direction::both });
		eddm_26l.ticks.push_back({ 0.5, 90, 0, 15, Direction::both });
		eddm_26l.ticks.push_back({ 0.5, 90, 0, 22, Direction::both });
		eddm_26l.ticks.push_back({ 0.3, 45, 0, 13.05, Direction::left });
		eddm_26l.ticks.push_back({ 0.3, 45, 0, 16.05, Direction::left });
		eddm.runways.push_back(eddm_26l);

		Runway_Settings eddm_08l;
		eddm_08l.fix = "MAGAT";
		eddm_08l.runway = "08L";
		eddm_08l.ticks.push_back({ 0.5, 90, 0.5, 4, Direction::left });
		eddm_08l.ticks.push_back({ 0.5, 90, 0, 12, Direction::both });
		eddm_08l.ticks.push_back({ 0.5, 90, 0, 15, Direction::both });
		eddm_08l.ticks.push_back({ 0.5, 90, 0, 22, Direction::both });
		eddm_08l.ticks.push_back({ 0.3, 45, 0, 13.05, Direction::left });
		eddm_08l.ticks.push_back({ 0.3, 45, 0, 16.05, Direction::left });
		eddm.runways.push_back(eddm_08l);

		Special_Settings spc_08r;
		spc_08r.distance_cl = 1.755;
		spc_08r.distance_thr1 = 13.05;
		spc_08r.distance_thr2 = 16.05;
		spc_08r.length = 12;
		spc_08r.direction = Direction::right;

		Runway_Settings eddm_08r;
		eddm_08r.fix = "BEGEN";
		eddm_08r.runway = "08R";
		eddm_08r.ticks.push_back({ 0.5, 90, 0.5, 4, Direction::right });
		eddm_08r.ticks.push_back({ 0.5, 90, 0, 12, Direction::both });
		eddm_08r.ticks.push_back({ 0.5, 90, 0, 15, Direction::both });
		eddm_08r.ticks.push_back({ 0.5, 90, 0, 18, Direction::both });
		eddm_08r.ticks.push_back({ 0.5, 90, 0, 22, Direction::both });
		eddm_08r.ticks.push_back({ 0.3, 45, 0, 13.05, Direction::right });
		eddm_08r.ticks.push_back({ 0.3, 45, 0, 16.05, Direction::right });
		eddm_08r.special = spc_08r;
		eddm.runways.push_back(eddm_08r);

		return eddm;
	}
};

struct EDMA
{
	Airport_Settings get()
	{
		Airport_Settings edma;
		edma.airport = "EDMA";

		Line_Settings lines;
		lines.distance_thr = 0;
		lines.length_gap = 1;
		lines.length_line = 1;
		lines.repeats = 6;
		lines.starts_with_line = false;
		edma.lines.push_back(lines);

		Runway_Settings edma_25;
		edma_25.fix = "BUREG";
		edma_25.runway = "25";
		edma_25.ticks.push_back({ 0.5, 90, 0.5, 6.1, Direction::both });
		edma_25.ticks.push_back({ 0.5, 90, 0.5, 10, Direction::both });
		edma.runways.push_back(edma_25);

		Runway_Settings edma_07;
		edma_07.fix = "ABUSI";
		edma_07.runway = "07";
		edma_07.ticks.push_back({ 0.5, 90, 0.5, 7.1, Direction::both });
		edma_07.ticks.push_back({ 0.5, 90, 0.5, 10, Direction::both });
		edma.runways.push_back(edma_07);

		return edma;
	}
};

struct EDMO
{
	Airport_Settings get()
	{
		Airport_Settings edmo;
		edmo.airport = "EDMO";

		Line_Settings lines;
		lines.distance_thr = 0;
		lines.length_gap = 1;
		lines.length_line = 1;
		lines.repeats = 5;
		lines.starts_with_line = false;
		edmo.lines.push_back(lines);

		Runway_Settings edmo_22;
		edmo_22.fix = "EDIMO";
		edmo_22.runway = "22";
		edmo_22.ticks.push_back({ 0.5, 90, 0.5, 7.4, Direction::both });
		edmo_22.ticks.push_back({ 0.5, 90, 0.5, 10, Direction::both });
		edmo.runways.push_back(edmo_22);

		Runway_Settings edmo_04;
		edmo_04.runway = "04";
		edmo_04.lines.push_back(Line_Settings{ 0 });
		edmo.runways.push_back(edmo_04);

		return edmo;
	}
};

struct ZZZZ
{
	Airport_Settings get()
	{
		Airport_Settings zzzz;
#ifndef __EDDM
		Line_Settings zzzz_line;
		zzzz_line.distance_thr = 0;
		zzzz_line.length_gap = 1;
		zzzz_line.length_line = 1;
		zzzz_line.repeats = 10;
		zzzz_line.starts_with_line = false;
		zzzz.lines.push_back(zzzz_line);
		zzzz.ticks.push_back({ 0.5, 90, 0.5, 4, Direction::both });
		zzzz.ticks.push_back({ 0.5, 90, 0.5, 10, Direction::both });
		zzzz.ticks.push_back({ 0.5, 90, 0.5, 20, Direction::both });
#endif
		return zzzz;
	}
};

static EDDM eddm;
static EDMA edma;
static EDMO edmo;
static ZZZZ zzzz;

struct Data
{
	Data() 
	{ 
		apts.push_back(eddm.get());
		apts.push_back(edma.get());
		apts.push_back(edmo.get());
	}
	std::list<Airport_Settings> apts;
};