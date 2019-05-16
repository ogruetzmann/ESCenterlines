#pragma once
#include <string>
#include "Coordinate.h"
class Runway
{
public:
	Runway(std::string &&airport, std::string &&runway, Coordinate thresh, Coordinate stop, bool active_arr, bool active_dep);
	~Runway();

	const std::string runway_designator;
	const std::string airport_designator;
	const Coordinate runway_threshold{ 0,0 };
	bool active_arrival{ false };
	bool active_departure{ false };
	double runway_track{ 0 };

};

