#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <GeographicLib\GeodesicLine.hpp>
#include "EuroScopePlugIn.h"
#include "Runway.h"
#include "Line.h"

class CESCenterlinesScreen :
	public EuroScopePlugIn::CRadarScreen
{
public:
	static constexpr double meters_per_nm { 1852 };

	explicit CESCenterlinesScreen(FILETIME & sTime);
	virtual ~CESCenterlinesScreen();

	void OnRefresh(HDC hDC, int Phase);
	void OnAsrContentLoaded(bool Loaded);

	inline void OnAsrContentToBeClosed()
	{
		delete this;
	}

	inline double InNM(double distance_in_nautical_miles)
	{
		return distance_in_nautical_miles * meters_per_nm;
	}

private:
	const FILETIME & ActiveRunwaysUpdateTime;
	FILETIME ActiveRunwaysLastUpdateTime;
	std::vector <std::unique_ptr<CRunway>> runway_settings;
	std::vector <std::unique_ptr<CLine>> lines;

	void DrawExtendedCenterlines(HDC & hdc);

	void LoadRunwayData();
	void LoadRunwayUserSettings(CRunway * runway);

	void CreateCenterlines();

	bool IsDataUpdated() const;
	void DisplayMessage(std::string message);
};

