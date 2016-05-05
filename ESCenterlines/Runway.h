#pragma once
#include <Windows.h>
#include <string>
#include <EuroScopePlugIn.h>
#include "Coordinate.h"
#include "Identifier.h"

class CRunway
{
public:
	CRunway(Identifier& id, EuroScopePlugIn::CSectorElement& se, int index);
	virtual ~CRunway();

	int GetApproachCourse() const;
	const Identifier& GetId() const;
	const CCoordinate& GetStopEndPosition() const;
	const CCoordinate& GetThresholdPosition() const;
	void Init(EuroScopePlugIn::CSectorElement& se, int index);
	
private:
	int course { 0 };
	CCoordinate threshold { 0.0, 0.0 };
	CCoordinate stop_end { 0.0, 0.0 };
	Identifier id;
};

