#include "Runway.h"



CRunway::CRunway()
{
}


CRunway::~CRunway()
{
}

bool CRunway::CalculateHeading()
{
	if (latitude_RunwayThreshold[0] && latitude_RunwayThreshold[1] && rlon_RunwayThreshold[0] && rlon_RunwayThreshold[1])
	{
		return true;
	}
	return false;
}

inline const std::string & CRunway::GetAirportName() const
{
	return sAirportName;
}

inline const std::string & CRunway::GetDesignator(int index) const
{
	return sRunwayDesignator[index];
}

void CRunway::GetThreshold(double & latitude, double & longitude, int index)
{
	latitude = latitude_RunwayThreshold[index];
	longitude = latitude_RunwayThreshold[index];
}

void CRunway::GetThreshold(EuroScopePlugIn::CPosition & position, int index)
{
	position.m_Latitude = latitude_RunwayThreshold[index];
	position.m_Longitude = rlon_RunwayThreshold[index];
}

void CRunway::SetAirportName(const std::string & name)
{
	if (name.length() > 4)
	{
		sAirportName = name.substr(4);
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
		sRunwayDesignator[index] = designator.substr(3);
	}
	else
	{
		sRunwayDesignator[index] = designator;
	}
}

void CRunway::SetThreshold(double latitude, double longitude, int index)
{
	latitude_RunwayThreshold[index] = latitude;
	rlon_RunwayThreshold[index] = longitude;
	CalculateHeading();
}

void CRunway::SetThreshold(EuroScopePlugIn::CPosition & position, int index)
{
	latitude_RunwayThreshold[index] = position.m_Latitude;
	rlon_RunwayThreshold[index] = position.m_Longitude;
	CalculateHeading();
}
