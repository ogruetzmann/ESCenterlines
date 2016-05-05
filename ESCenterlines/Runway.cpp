#include "Runway.h"

CRunway::CRunway()
{
	
}

CRunway::~CRunway()
{
}

std::unique_ptr<CRunway> CRunway::CreateRunway(EuroScopePlugIn::CSectorElement & se, int index)
{
	if (!se.IsValid() ||
		!(se.GetElementType() == EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
		return nullptr;

	std::unique_ptr<CRunway> rwy = std::make_unique<CRunway>();
	
	se.GetPosition(&rwy->threshold, index);
	se.GetPosition(&rwy->stop_end, (index ? 0 : 1));
	rwy->identifier.runway_designator = se.GetRunwayName(index);
	rwy->sectorfile_approach_course = se.GetRunwayHeading(index);
	rwy->is_active = se.IsElementActive(false, index);
	rwy->SetAirportString(se.GetAirportName());

	return rwy;
}

double CRunway::GetApproachCourse(CourseType type)
{
	if (type == CourseType::preset)
		type = course_type;

	if (type == CourseType::sectorfile)
		return sectorfile_approach_course;
	if (type == CourseType::sectorfile_if_available)
	{
		if (sectorfile_approach_course)
			return sectorfile_approach_course;
		else
			return calculated_approach_course; //geographic.GetAzimuth(threshold, stop_end);
	}
	return calculated_approach_course; // geographic.GetAzimuth(threshold, stop_end);
}

void CRunway::SetExtendedCenterline(const CExtendedCenterline & ecl)
{
	extended_centerline = ecl;
}

void CRunway::SetAirportString(const char * designator)
{
	identifier.airport_designator = designator;
	if (identifier.airport_designator.size() > 4)
		identifier.airport_designator.resize(4);
}