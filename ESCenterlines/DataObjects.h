#pragma once
#include <memory>

enum class Direction
{
	both, left, right
};

enum class Course
{
	Sectorfile, Calculated
};

struct CenterlineMarker
{
	double angle;
	double dist_thr;
	double dist_cl;
	double length;
	Direction direction;
	std::unique_ptr<Identifier> depends_on;
};

struct CenterlineElement
{
	double dash_length;
	double gap_length;
	int number;
	bool starts_gap;
};