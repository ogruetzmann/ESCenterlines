#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include <EuroScopePlugIn.h>
#include "Geographic.h"
#include "Coordinate.h"
#include "ExtendedCenterline.h"

enum class CourseType
{
	preset, sectorfile, sectorfile_if_available, calculated
};

class CRunway
{
public:
	bool is_active { false };
	int sectorfile_approach_course { 0 };
	CCoordinate threshold { 0.0, 0.0 };
	CCoordinate stop_end { 0.0, 0.0 };
	std::string runway_designator;
	std::string final_approach_fix;
	std::string airport_designator;
	CourseType course_type { CourseType::calculated };
	CExtendedCenterline extended_centerline;


	CRunway();
	virtual ~CRunway();
	static std::unique_ptr<CRunway> CreateRunway(EuroScopePlugIn::CSectorElement & se, int index);

	double GetApproachCourse(CourseType ct = CourseType::preset);

	void SetFinalApproachFix(const std::string & faf);
	
private:
	CGeographic geographic;
	void SetAirportString(const char * designator);
};

