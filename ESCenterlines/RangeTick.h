#pragma once

enum class Direction
{
	both, left, right
};

class CRangeTick
{
public:
	double distance_thr { 0.0 };
	double distance_cl { 0.0 };
	double length { 0.0 };
	double angle { 0.0 };
	Direction direction;

	CRangeTick(double distance_from_threshold, double distance_from_centerline, double length, Direction d = Direction::both, double angle = 0.0);
	virtual ~CRangeTick();
};

