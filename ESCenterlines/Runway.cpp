#include "Runway.h"



CRunway::CRunway()
{
}


CRunway::~CRunway()
{
}

bool CRunway::CalculateRunwayHeading()
{
	if (RunwayThreshold_latitude[0] && RunwayThreshold_latitude[1] && RunwayThreshold_longitude[0] && RunwayThreshold_longitude[1])
	{
		static GeographicLib::Geodesic g(GeographicLib::Geodesic::WGS84());
		GeographicLib::GeodesicLine gl = g.InverseLine(RunwayThreshold_latitude[0], RunwayThreshold_longitude[0], RunwayThreshold_latitude[1], RunwayThreshold_longitude[1]);
		rRunwayHeading[0] = gl.Azimuth();
		rRunwayHeading[1] = rRunwayHeading[0] > 180.0 ? rRunwayHeading[0] - 180.0 : rRunwayHeading[0] + 180.0;
		return true;
	}
	return false;
}

const std::string & CRunway::GetAirportName() const
{
	return sAirportName;
}

const std::string & CRunway::GetDesignator(int index) const
{
	return sRunwayDesignator[index];
}

void CRunway::GetThreshold(double & latitude, double & longitude, int index)
{
	latitude = RunwayThreshold_latitude[index];
	longitude = RunwayThreshold_latitude[index];
}

void CRunway::GetThreshold(EuroScopePlugIn::CPosition & position, int index)
{
	position.m_Latitude = RunwayThreshold_latitude[index];
	position.m_Longitude = RunwayThreshold_longitude[index];
}

double CRunway::GetHeading(int index) const
{
	return rRunwayHeading[index];
}

bool CRunway::GetActive(int index) const
{
	return isActive[index];
}

void CRunway::SetAirportName(const std::string & name)
{
	if (name.length() > 4)
	{
		sAirportName = name.substr(0,4);
	}
	else
	{
		sAirportName = name;
	}
}

void CRunway::SetDesignator(const std::string & designator, int index)
{
	if (designator.length() > 3)
	{
		sRunwayDesignator[index] = designator.substr(0,3);
	}
	else
	{
		sRunwayDesignator[index] = designator;
	}
}

void CRunway::SetThreshold(double latitude, double longitude, int index)
{
	RunwayThreshold_latitude[index] = latitude;
	RunwayThreshold_longitude[index] = longitude;
	CalculateRunwayHeading();
}

void CRunway::SetThreshold(EuroScopePlugIn::CPosition & position, int index)
{
	RunwayThreshold_latitude[index] = position.m_Latitude;
	RunwayThreshold_longitude[index] = position.m_Longitude;
	CalculateRunwayHeading();
}

void CRunway::SetActive(bool active, int index)
{
	isActive[index] = active;
}
