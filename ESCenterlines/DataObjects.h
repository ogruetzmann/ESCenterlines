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
	CenterlineMarker(double dist_thr, double dist_cl, double length, double angle, Direction direction, std::unique_ptr<Identifier> depends_on = nullptr)
		: dist_thr(dist_thr), dist_cl(dist_cl), length(length), angle(angle), direction(direction), depends_on(std::move(depends_on))
	{}
	CenterlineMarker(const CenterlineMarker& m)
		: dist_thr(m.dist_thr), dist_cl(m.dist_cl), length(m.length), angle(m.angle), direction(m.direction), depends_on(m.depends_on.get())
	{}
	double dist_thr { 0.0 };
	double dist_cl { 0.0 };
	double length { 0.0 };
	double angle { 0.0 };
	Direction direction { Direction::both };
	std::unique_ptr<Identifier> depends_on { nullptr };
};

struct CenterlineElement
{
	CenterlineElement(double dash_length, double gap_length, int number, bool starts_gap = true)
		: dash_length(dash_length), gap_length(gap_length), number(number), starts_gap(starts_gap)
	{}
	double dash_length;
	double gap_length;
	int number;
	bool starts_gap;
};