#include "Runway.h"



CRunway::CRunway()
{
}

CRunway::CRunway(EuroScopePlugIn::CSectorElement & se, int index)
{
	if (!se.IsValid() ||
		!(se.GetElementType() == EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
		return;

	se.GetPosition(&threshold, index);
	se.GetPosition(&stop_end, (index == 1 ? 0 : 1));
	runway_designator = se.GetRunwayName(index);
	sectorfile_approach_course = se.GetRunwayHeading(index);
	is_active = se.IsElementActive(false, index);
	SetAirportString(se.GetAirportName());
}

CRunway::~CRunway()
{
}

void CRunway::SetFinalApproachFix(std::string & faf)
{
	final_approach_fix = faf;
}

void CRunway::SetAirportString(const char * designator)
{
	airport_designator = designator;
	if (airport_designator.size() > 4)
		airport_designator.resize(4);
}


