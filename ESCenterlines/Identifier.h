#pragma once
#include <string>

class Identifier
{
public:
	Identifier(const std::string & airport, const std::string & runway);
	Identifier(const Identifier& id);
	bool operator==(const Identifier& id) const;
private:
	std::string airport_designator {};
	std::string runway_designator {};
};