#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include <GeographicLib\Geodesic.hpp>
#include "EuroScopePlugIn.h"
#include "Coordinate.h"

enum class CourseType
{
	preset, sectorfile, sectorfile_if_available, calculated
};

class CRunway
{
public:
	bool is_active { false };
	int sectorfile_approach_course { -1 };
	CCoordinate threshold { 0.0, 0.0 };
	CCoordinate stop_end { 0.0, 0.0 };
	std::string runway_designator;
	std::string final_approach_fix;
	std::string airport_designator;
	CourseType course_type { CourseType::sectorfile };

	CRunway();
	virtual ~CRunway();
	static std::unique_ptr<CRunway> CreateRunway(EuroScopePlugIn::CSectorElement & se, int index);

	double GetCalculatedApproachCourse() const;
	int GetSectorfileApproachCourse() const;
	double GetApproachCourse(CourseType ct = CourseType::preset);

	void SetFinalApproachFix(std::string & faf);
	
private:
	void SetAirportString(const char * designator);
};

