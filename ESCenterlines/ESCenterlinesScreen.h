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
#include "DataObjects.h"

using GeographicLib::Constants;

class CESCenterlinesScreen :
	public EuroScopePlugIn::CRadarScreen
{
public:
	CESCenterlinesScreen(FILETIME& sTime, CCenterlineSettings& centerline_settings);
	virtual ~CESCenterlinesScreen();

	void OnAsrContentLoaded(bool Loaded);
	void OnAsrContentToBeClosed();
	void OnAsrContentToBeSaved(void);
	bool OnCompileCommand(const char* sCommandLine);
	void OnRefresh(HDC hDC, int Phase);

private:
	const char* DISPLAY_CENTERLINES = "Display_Centerlines";
	const char* DISPLAY_ACTIVE = "Display Active";

	const FILETIME& ActiveRunwaysUpdateTime;
	FILETIME ActiveRunwaysLastUpdateTime;
	bool display_centerlines { true };
	bool display_active { true };
	
	std::vector<std::unique_ptr<CRunway>> runways;
	std::vector<std::unique_ptr<CExtendedCenterline>> centerlines;
	std::vector<Identifier> active_runways;
	std::vector<CLine> lines;

	CGeographic geographic;
	CCenterlineSettings& centerline_settings;
	
	void CreateLines();
	void DrawExtendedCenterlines(HDC& hdc);
	void DrawLines(HDC& hDC);
	std::unique_ptr<CCoordinate> GetFixCoordinate(const std::string& name, const CCoordinate& threshold);
	void InitAsrSettings();
	bool IsDataUpdated() const;
	bool IsRunwayActive(const Identifier& id) const;
	void LoadRunwayData();
	void RefreshActiveRunways();

	void DisplayMessage(std::string message);
};

