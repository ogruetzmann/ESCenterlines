#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <GeographicLib\GeodesicLine.hpp>
#include <GeographicLib\Constants.hpp>
#include "EuroScopePlugIn.h"
#include "Runway.h"
#include "MyPosition.h"


class CESCenterlinesScreen :
	public EuroScopePlugIn::CRadarScreen
{
private:
	FILETIME & ActiveRunwaysUpdateTime;
	FILETIME ActiveRunwaysLastUpdateTime;

	std::vector<CRunway> * Runways = 0;

public:
	CESCenterlinesScreen(FILETIME & sTime);
	virtual ~CESCenterlinesScreen();

	void OnRefresh(HDC hDC, int Phase);
	void OnAsrContentLoaded(bool Loaded);


	inline void OnAsrContentToBeClosed(void)
	{
		delete this;
	}

private:
	void LoadRunways();
};

