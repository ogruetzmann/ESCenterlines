#pragma once
#include <GeographicLib\Geodesic.hpp>
#include "Runway.h"
#include "Line.h"
#include "ExtendedCenterline.h"
#include "Coordinate.h"

class CGeographic
{
public:
	CGeographic();
	~CGeographic();

	void CalculateExtendedCenterline(const CRunway& runway, CExtendedCenterline* cl, const CCoordinate * coordinate, std::vector<std::unique_ptr<CLine>>& l);
	double GetAzimuth(const CCoordinate & c1, const CCoordinate & c2);
	CCoordinate GetCoordinate(CCoordinate c1, double azimuth, double distance);
	double GetDistance(const CCoordinate & c1, const CCoordinate & c2);

private:
	GeographicLib::Geodesic geodesic;

	void CalculateCenterline(const CRunway& rwy, const CExtendedCenterline* cl, std::vector<std::unique_ptr<CLine>>& l, double course);
	void CalculateRangeTicks(const CRunway& runway, const CExtendedCenterline* centerline, std::vector<std::unique_ptr<CLine>>& l, double course);
	double CalculateApproachCourse(const CRunway& runway, const CCoordinate* coordinate);
};

