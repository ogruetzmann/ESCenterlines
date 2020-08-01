#pragma once
#include <EuroScopePlugIn.h>
struct Coordinate
	: public EuroScopePlugIn::CPosition
{
	Coordinate() {};
	Coordinate(double lat, double lon)
	{
		m_Latitude = lat;
		m_Longitude = lon;
	}
	double &latitude = m_Latitude;
	double &longitude = m_Longitude;
};

struct Coordinate2
	: public EuroScopePlugIn::CPosition
{

};

struct CLine
{
	Coordinate start;
	Coordinate end;
	bool apt{ false };
	bool rwy{ false };
};
