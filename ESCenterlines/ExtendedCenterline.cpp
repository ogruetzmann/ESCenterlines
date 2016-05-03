#include "ExtendedCenterline.h"



CExtendedCenterline::CExtendedCenterline()
{
}

CExtendedCenterline::~CExtendedCenterline()
{
}

void CExtendedCenterline::SetDefault()
{
	range_ticks.push_back(CRangeTick(4, 0.5, 0.5, Direction::both));
	range_ticks.push_back(CRangeTick(10, 0.5, 0.5, Direction::both));
	range_ticks.push_back(CRangeTick(20, 0.5, 0.5, Direction::both));
	centerline_elements.push_back(CCenterlineElement(1, 1, 10, true));
	centerline_elements.push_back(CCenterlineElement(4, 1, 2, true));
}

