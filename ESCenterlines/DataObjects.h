#pragma once
#include <string>

struct identifier
{
	inline bool operator==(identifier& id) const
	{
		return id.airport_designator == airport_designator 
			&& id.runway_designator == runway_designator;
	}
	std::string airport_designator;
	std::string runway_designator;
};