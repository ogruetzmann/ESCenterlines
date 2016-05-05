#include "Line.h"

CLine::CLine(const Identifier& id, const CCoordinate & coordinate1,const CCoordinate & coordinate2)
	: id(id), c1(coordinate1), c2(coordinate2)
{
}

CLine::CLine(const Identifier& id, const CCoordinate & coordinate1, const CCoordinate & coordinate2, Identifier& depends_on)
	: id(id), c1(coordinate1), c2(coordinate2), depends_on(std::make_unique<Identifier>(depends_on))
{
}

CLine::CLine(const CLine& cl)
	: id(cl.id), c1(cl.c1), c2(cl.c2), depends_on(cl.depends_on.get())
{
}

CLine::~CLine()
{
}

const CCoordinate& CLine::C1() const
{
	return c1;
}

const CCoordinate& CLine::C2() const
{
	return c2;
}

const Identifier* CLine::DependsOn() const
{
	//return nullptr;
	return depends_on.get();
}

const Identifier& CLine::GetId() const
{
	return id;
}
