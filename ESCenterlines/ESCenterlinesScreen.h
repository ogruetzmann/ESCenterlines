#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <EuroScopePlugIn.h>
#include "Geographic.h"
#include "Runway.h"
#include "Line.h"
#include "ExtendedCenterline.h"

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
	bool OnCompileCommand(const char * sCommandLine);

	inline double InNM(double distance_in_nautical_miles)
	{
		return distance_in_nautical_miles * meters_per_nm;
	}

private:
	const char * DISPLAY_CENTERLINES = "Display_Centerlines";
	const char * DISPLAY_ACTIVE = "Display Active";
	const FILETIME & ActiveRunwaysUpdateTime;
	FILETIME ActiveRunwaysLastUpdateTime;
	std::vector <std::unique_ptr<CRunway>> runways;
	std::vector <std::unique_ptr<CLine>> lines;
	CGeographic geographic;
	bool display_centerlines { true };
	bool display_active { true };

	void DrawExtendedCenterlines(HDC & hdc);

	void CalculateCenterline(CRunway & runway);
	void CalculateRangeTicks(CRunway & runway);
	void CreateCenterlines();
	void InitAsrSettings();
	void LoadRunwayData();
	void LoadRunwayUserData();
	void RefreshData();

	bool IsDataUpdated() const;
	void DisplayMessage(std::string message);
};

