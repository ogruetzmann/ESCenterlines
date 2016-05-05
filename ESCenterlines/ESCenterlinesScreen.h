#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <EuroScopePlugIn.h>
#include "Geographic.h"
#include "Runway.h"
#include "Line.h"
#include "CenterlineSettings.h"
#include "DataObjects.h"

using GeographicLib::Constants;

class CESCenterlinesScreen :
	public EuroScopePlugIn::CRadarScreen
{
public:
	CESCenterlinesScreen(FILETIME & sTime, CCenterlineSettings & centerline_settings);
	virtual ~CESCenterlinesScreen();

	void OnRefresh(HDC hDC, int Phase);
	void OnAsrContentLoaded(bool Loaded);

	inline void OnAsrContentToBeClosed()
	{
		delete this;
	}
	void OnAsrContentToBeSaved(void);
	bool OnCompileCommand(const char * sCommandLine);

private:
	const char * DISPLAY_CENTERLINES = "Display_Centerlines";
	const char * DISPLAY_ACTIVE = "Display Active";
	const FILETIME & ActiveRunwaysUpdateTime;
	FILETIME ActiveRunwaysLastUpdateTime;
	std::vector <std::unique_ptr<CRunway>> runways;
	CGeographic geographic;
	bool display_centerlines { true };
	bool display_active { true };
	CCenterlineSettings & centerline_settings;
	std::vector<Identifier> active_runways;

	void DrawExtendedCenterlines(HDC & hdc);
	void DrawLines(HDC &hDC, const std::vector<CLine> & lines);

	std::unique_ptr<CCoordinate> GetFixCoordinate(const std::string & name, const CCoordinate & threshold);
	void InitAsrSettings();
	bool IsRunwayActive(const Identifier & id) const;
	void LoadRunwayData();
	void RefreshActiveRunways();

	bool IsDataUpdated() const;
	void DisplayMessage(std::string message);
};

