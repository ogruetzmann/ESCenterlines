#include "RangeTick.h"

CRangeTick::CRangeTick(double distance_from_threshold, double distance_from_centerline, double length, Direction direction, double angle)
	: distance_thr(distance_from_threshold), distance_cl(distance_from_centerline), length(length), direction(direction), angle(angle)
{
}


CRangeTick::~CRangeTick()
{
}
