#pragma once
#include <memory>
#include "Coordinate.h"
#include "Identifier.h"


class CLine
{
public:
	CLine(const Identifier& id, const CCoordinate & coordinate1, const CCoordinate & coordinate2);
	CLine(const Identifier& id, const CCoordinate & coordinate1, const CCoordinate & coordinate2, Identifier& depends_on);
	CLine(const CLine& cl);
	virtual ~CLine();

	const CCoordinate& C1() const;
	const CCoordinate& C2() const;
	const Identifier* DependsOn() const;
	const Identifier& GetId() const;
	
private:
	CCoordinate c1 { 0.0, 0.0 };
	CCoordinate c2 { 0.0, 0.0 };
	Identifier id;
	std::unique_ptr<Identifier> depends_on { nullptr };

};
