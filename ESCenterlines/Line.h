#pragma once
#include <memory>
#include <string>
#include "Coordinate.h"
#include "DataObjects.h"


class CLine
{
public:
	CCoordinate c1;
	CCoordinate c2;
	Identifier depends_on;
	bool depending { false };

	CLine();
	CLine(const CCoordinate & coordinate1, const CCoordinate & coordinate2);
	CLine(const CCoordinate & coordinate1, const CCoordinate & coordinate2, std::string & airport, std::string & runway);
	CLine(const CCoordinate & coordinate1, const CCoordinate & coordinate2, Identifier & id);
	virtual ~CLine();

	bool DependsOn(Identifier& id);
	void DependsOn(const std::string & airport, const std::string & runway);
};
