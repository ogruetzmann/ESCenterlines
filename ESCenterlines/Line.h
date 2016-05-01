#pragma once
#include "Coordinate.h"

class CLine
{
public:
	CCoordinate c1;
	CCoordinate c2;

	CLine();
	CLine(CCoordinate & coordinate1, CCoordinate & coordinate2);
	virtual ~CLine();
};

