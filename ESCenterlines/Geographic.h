#pragma once
#include <GeographicLib\Geodesic.hpp>
#include "Runway.h"
#include "Coordinate.h"

class CGeographic
{
public:
	CGeographic();
	~CGeographic();

	void CalculateExtendedCenterline(CRunway & runway, CCoordinate * coordinate);
	double GetAzimuth(const CCoordinate & c1, const CCoordinate & c2);
	CCoordinate GetCoordinate(CCoordinate c1, double azimuth, double distance);
	double GetDistance(const CCoordinate & c1, const CCoordinate & c2);

private:
	GeographicLib::Geodesic geodesic;

	void CalculateCenterline(CRunway & runway);
	void CalculateRangeTicks(CRunway & runway);
	void CalculateApproachCourse(CRunway & runway, CCoordinate * coordinate);
};

