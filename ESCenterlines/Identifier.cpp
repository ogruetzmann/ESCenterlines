#include "Identifier.h"

Identifier::Identifier(const std::string& airport, const std::string& runway)
	: airport_designator(airport), runway_designator(runway)
{
	airport_designator.resize(4);
}

Identifier::Identifier(const Identifier& id)
: airport_designator(id.airport_designator), runway_designator(id.runway_designator)
{
	airport_designator.resize(4);
}

bool Identifier::operator==(const Identifier& id) const
{
	return id.airport_designator == airport_designator
		&& id.runway_designator == runway_designator;
}

