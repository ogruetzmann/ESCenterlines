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
	std::unique_ptr<identifier> depends_on;

	CLine();
	CLine(CLine &&);
	CLine(CCoordinate & coordinate1, CCoordinate & coordinate2, identifier * depends = nullptr);
	virtual ~CLine();
};
