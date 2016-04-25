#pragma once
#include "Coordinate.h"

class CLine
{
public:
	CCoordinate p1;
	CCoordinate p2;

	CLine();
	CLine(CCoordinate & point1, CCoordinate & point2);
	virtual ~CLine();
};

