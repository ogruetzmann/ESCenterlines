#include "Line.h"

CLine::CLine()
{
}

CLine::CLine(const CCoordinate & coordinate1,const CCoordinate & coordinate2)
	: c1(coordinate1), c2(coordinate2)
{
}

CLine::CLine(const CCoordinate & coordinate1, const CCoordinate & coordinate2, std::string & airport, std::string & runway)
	: c1(coordinate1), c2(coordinate2), depends_on(airport, runway), depending(true)
{
}

CLine::CLine(const CCoordinate & coordinate1, const CCoordinate & coordinate2, Identifier & id)
	: c1(coordinate1), c2(coordinate2), depends_on(id)
{
}

CLine::~CLine()
{
}

inline bool CLine::DependsOn(Identifier & id)
{
	if (depending)
		id = depends_on;
	return depending;
}

void CLine::DependsOn(const std::string & airport, const std::string & runway)
{
	depends_on.airport_designator = airport;
	depends_on.runway_designator = runway;
	depending = true;
}
