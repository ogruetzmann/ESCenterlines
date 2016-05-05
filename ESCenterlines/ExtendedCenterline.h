#pragma once
#include <vector>
#include <string>
#include "RangeTick.h"
#include "CenterlineElement.h"

class CExtendedCenterline
{
public:
	std::vector<CRangeTick> range_ticks;
	std::vector<CCenterlineElement> centerline_elements;
	std::string final_approach_fix {};
	double course { 0.0 };

	CExtendedCenterline();
	virtual ~CExtendedCenterline();

	void AddElement(CCenterlineElement & element);
	void AddElement(double tick_length, double gap_length, double length, bool starts_with_gap);
	void AddRangeTick(CRangeTick & rt);
	void AddRangeTick(double distance_from_threshold, double distance_from_centerline, double length, Direction direction = Direction::both, double angle = 0.0);
	void AddRangeTick(double distance_from_threshold, 
					  double distance_from_centerline, 
					  double length, 
					  std::string depends_airport, 
					  std::string depends_runway, 
					  Direction direction = Direction::both, 
					  double angle = 0.0);
	void SetDefault();
};

