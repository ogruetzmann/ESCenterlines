#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <EuroScopePlugIn.h>
#include "Coordinate.h"
#include "ExtendedCenterline.h"
#include "Line.h"
#include "DataObjects.h"

class CRunway
{
public:
	bool is_active { false };
	int sectorfile_approach_course { 0 };
	double calculated_approach_course { 0.0 };
	CCoordinate threshold { 0.0, 0.0 };
	CCoordinate stop_end { 0.0, 0.0 };
	Identifier identifier;
	CourseType course_type { CourseType::calculated };
	CExtendedCenterline extended_centerline;
	inline void AddLine(CLine && line)
	{
		lines.push_back(line);
	}
	inline const std::vector<CLine> & GetLines()
	{
		return lines;
	}
	

	CRunway();
	virtual ~CRunway();
	static std::unique_ptr<CRunway> CreateRunway(EuroScopePlugIn::CSectorElement & se, int index);

	double GetApproachCourse(CourseType ct = CourseType::preset);
	void SetExtendedCenterline(const CExtendedCenterline & extended_centerline);
	
private:
	std::vector<CLine> lines;
	void SetAirportString(const char * designator);
};

