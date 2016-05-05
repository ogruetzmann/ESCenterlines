#pragma once
#include <string>

struct Identifier
{
	Identifier()
	{
	}
	Identifier(const std::string & airport, const std::string & runway) : airport_designator(airport), runway_designator(runway)
	{
	}
	bool operator==(const Identifier& id) const
	{
		return id.airport_designator == airport_designator 
			&& id.runway_designator == runway_designator;
	}
	std::string airport_designator {};
	std::string runway_designator {};
};

enum class Direction
{
	both, left, right
};

enum class CourseType
{
	preset, sectorfile, sectorfile_if_available, calculated
};
