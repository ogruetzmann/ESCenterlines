#include "Geographic.h"



CGeographic::CGeographic() 
	: geodesic(GeographicLib::Geodesic::WGS84())
{
}


CGeographic::~CGeographic()
{
}

double CGeographic::GetAzimuth(const CCoordinate & c1, const CCoordinate & c2)
{
	double azi, azi2;
	geodesic.Inverse(c1.m_Latitude, c1.m_Longitude, c2.m_Latitude, c2.m_Longitude, azi, azi2);
	return azi;
}

CCoordinate CGeographic::GetCoordinate(CCoordinate c1, double azimuth, double distance)
{
	double lat2, lon2;
	geodesic.Direct(c1.m_Latitude, c1.m_Longitude, azimuth, distance, lat2, lon2);
	return CCoordinate(lat2, lon2);
}
