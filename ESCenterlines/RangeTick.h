#pragma once
#include <string>
#include "DataObjects.h"

class CRangeTick
{
public:
	double distance_thr { 0.0 };
	double distance_cl { 0.0 };
	double length { 0.0 };
	double angle { 0.0 };
	Direction direction { Direction::both };
	Identifier depends_on;
	bool depends { false };

	CRangeTick(double distance_from_threshold, double distance_from_centerline, double length, Direction d = Direction::both, double angle = 0.0);
	CRangeTick(double distance_from_threshold,
			   double distance_from_centerline,
			   double length,
			   Identifier && depends_on,
			   Direction d = Direction::both, 
			   double angle = 0.0);
	virtual ~CRangeTick();
};

