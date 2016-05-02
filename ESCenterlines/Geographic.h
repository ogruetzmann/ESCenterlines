#pragma once
#include <GeographicLib\Geodesic.hpp>
#include "Coordinate.h"

class CGeographic
{
public:
	CGeographic();
	~CGeographic();

	double GetAzimuth(const CCoordinate & c1, const CCoordinate & c2);
	CCoordinate GetCoordinate(CCoordinate c1, double azimuth, double distance);

private:
	GeographicLib::Geodesic geodesic;
};

