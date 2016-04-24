#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include <EuroScopePlugIn.h>
#include <GeographicLib\Math.hpp>
#include <GeographicLib\GeodesicLine.hpp>

class CRunway
{
private:
	std::string sAirportName;
	std::string sRunwayDesignator[2];
	GeographicLib::Math::real RunwayThreshold_latitude[2] = { 0.0, 0.0 };
	GeographicLib::Math::real RunwayThreshold_longitude[2] = { 0.0, 0.0 };
	GeographicLib::Math::real rRunwayHeading[2] = { 0.0, 0.0 };
	bool isActive[2] = { false, false };

public:
	CRunway();
	virtual ~CRunway();

	bool CalculateRunwayHeading();

	const std::string & GetAirportName() const;
	const std::string & GetDesignator(int index) const;
	void GetThreshold(double & latitude, double & longitude, int index);
	void GetThreshold(EuroScopePlugIn::CPosition & position, int index);
	double GetHeading(int index) const;
	bool GetActive(int index) const;

	void SetAirportName(const std::string & name);
	void SetDesignator(const std::string & designator, int index);
	void SetThreshold(double latitude, double longitude, int index);
	void SetThreshold(EuroScopePlugIn::CPosition & position, int index);
	void SetActive(bool active, int index);

};

