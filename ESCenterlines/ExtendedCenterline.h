#pragma once
#include <vector>
#include "RangeTick.h"
#include "CenterlineElement.h"


class CExtendedCenterline
{
public:
	std::vector<CRangeTick> range_ticks;
	std::vector<CCenterlineElement> centerline_elements;

	CExtendedCenterline();
	virtual ~CExtendedCenterline();
};

