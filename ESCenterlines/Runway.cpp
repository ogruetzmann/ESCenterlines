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
	rwy->runway_designator = se.GetRunwayName(index);
	rwy->sectorfile_approach_course = se.GetRunwayHeading(index);
	rwy->is_active = se.IsElementActive(false, index);
	rwy->SetAirportString(se.GetAirportName());

	return rwy;
}

double CRunway::GetCalculatedApproachCourse() const
{
	static GeographicLib::Geodesic gd(GeographicLib::Geodesic::WGS84());
	static double azi1, azi2;
	gd.Inverse(threshold.m_Latitude, threshold.m_Longitude, stop_end.m_Latitude, stop_end.m_Longitude, azi1, azi2);
	return azi1;
}

inline int CRunway::GetSectorfileApproachCourse() const
{
	return sectorfile_approach_course;
}

double CRunway::GetApproachCourse(CourseType type)
{
	if (type != CourseType::preset)
		course_type = type;

	if (course_type == CourseType::sectorfile)
		return sectorfile_approach_course;
	if (course_type == CourseType::sectorfile_if_available)
		return 0;
	else
		return GetCalculatedApproachCourse();
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