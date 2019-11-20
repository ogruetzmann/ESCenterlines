#pragma once
#include <EuroScopePlugIn.h>
struct Coordinate
{
	double latitude;
	double longitude;
	EuroScopePlugIn::CPosition cposition() 
	{
		EuroScopePlugIn::CPosition pos;
		pos.m_Latitude = latitude;
		pos.m_Longitude = longitude;
		return pos;
	}
};

struct CLine
{
	Coordinate start;
	Coordinate end;
};

