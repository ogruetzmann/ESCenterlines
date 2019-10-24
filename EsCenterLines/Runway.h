#pragma once
#include <string>
#include "Coordinate.h"
struct Runway
{
public:
	std::string runway_designator{};
	std::string airport_name{};
	Coordinate runway_threshold{ 0,0 };
	Coordinate runway_end{ 0,0 };
	bool active_arrival{ false };
	bool active_departure{ false };
	double runway_heading{ 0.0 };
	double runway_heading_calculated{ 0.0 };
	Coordinate endpoint{ 0,0 };
};
