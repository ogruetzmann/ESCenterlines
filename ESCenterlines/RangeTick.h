#pragma once

class CRangeTick
{
public:
	double distance_thr = 0.0;
	double distance_cl = 0.0;
	double length = 0.0;
	double angle = 0.0;

	CRangeTick();
	virtual ~CRangeTick();
};

