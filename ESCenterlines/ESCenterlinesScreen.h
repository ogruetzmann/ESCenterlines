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
#include "CenterlineSettings.h"

using GeographicLib::Constants;

class CESCenterlinesScreen :
	public EuroScopePlugIn::CRadarScreen
{
public:
	static constexpr double meters_per_nm { 1852 };

	explicit CESCenterlinesScreen(FILETIME & sTime, CCenterlineSettings & centerline_settings);
	virtual ~CESCenterlinesScreen();

	void OnRefresh(HDC hDC, int Phase);
	void OnAsrContentLoaded(bool Loaded);

	inline void OnAsrContentToBeClosed()
	{
		delete this;
	}
	bool OnCompileCommand(const char * sCommandLine);

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
	CCenterlineSettings & centerline_settings;

	void DrawExtendedCenterlines(HDC & hdc);

	void CalculateCenterline(CRunway & runway);
	void CalculateRangeTicks(CRunway & runway);
	void CreateCenterlines();
	void InitAsrSettings();
	void LoadRunwayData();
	void LoadRunwayUserData(CRunway & runway);
	void RefreshData();

	bool IsDataUpdated() const;
	void DisplayMessage(std::string message);
};

