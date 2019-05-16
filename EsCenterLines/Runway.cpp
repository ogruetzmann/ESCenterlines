#include "Runway.h"

Runway::Runway(std::string &&airport, std::string &&runway, Coordinate threshold, Coordinate stop, bool active_arr, bool active_dep)
	: airport_designator(airport), runway_designator(runway), runway_threshold(threshold), active_arrival(active_arr), active_departure(active_dep)
{

}

Runway::~Runway()
{
}
