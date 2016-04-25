#pragma once
#include <Windows.h>
#include <string>
#include "EuroScopePlugIn.h"
#include "Coordinate.h"


class CRunway
{
public:
	CCoordinate threshold = { 0.0, 0.0 };
	CCoordinate stop_end = { 0.0, 0.0 };
	int sectorfile_approach_course = 0;
	std::string runway_designator;
	std::string final_approach_fix;
	std::string airport_designator;
	bool is_active = false;

	CRunway();
	CRunway(EuroScopePlugIn::CSectorElement & se, int index);
	virtual ~CRunway();

	void SetFinalApproachFix(std::string & faf);

private:
	void SetAirportString(const char * designator);

};

