#include "Runway.h"

CRunway::CRunway(Identifier& id, EuroScopePlugIn::CSectorElement& se, int i)
	: id(id)
{
	Init(se, i);
}

CRunway::~CRunway()
{
}

int CRunway::GetApproachCourse() const
{
	return course;
}

const Identifier& CRunway::GetId() const
{
	return id;
}

const CCoordinate& CRunway::GetStopEndPosition() const
{
	return stop_end;
}

const CCoordinate& CRunway::GetThresholdPosition() const
{
	return threshold;
}

void CRunway::Init(EuroScopePlugIn::CSectorElement& se, int i)
{
	course = se.GetRunwayHeading(i);
	se.GetPosition(&threshold, i);
	se.GetPosition(&stop_end, (i ? 0 : 1));
}
