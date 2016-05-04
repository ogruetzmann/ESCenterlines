#include "Line.h"

CLine::CLine()
{
}

CLine::CLine(CLine && c) 
	: c1(c.c1), c2(c.c2), depends_on(std::move(c.depends_on.get()))
{
}

CLine::CLine(CCoordinate & coordinate1, CCoordinate & coordinate2, identifier * depends)
	: c1(coordinate1), c2(coordinate2), depends_on(nullptr)
{
}

CLine::~CLine()
{
}
