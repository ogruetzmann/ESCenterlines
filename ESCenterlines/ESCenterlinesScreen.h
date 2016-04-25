#pragma once
#include <Windows.h>
#include <string>
#include "EuroScopePlugIn.h"
#include "Runway.h"

class CESCenterlinesScreen :
	public EuroScopePlugIn::CRadarScreen
{
public:
	explicit CESCenterlinesScreen(FILETIME & sTime);
	virtual ~CESCenterlinesScreen();

	void OnRefresh(HDC hDC, int Phase);
	void OnAsrContentLoaded(bool Loaded);

	inline void OnAsrContentToBeClosed()
	{
		delete this;
	}

private:
	const FILETIME & ActiveRunwaysUpdateTime;
	FILETIME ActiveRunwaysLastUpdateTime;

	void DrawExtendedCenterlines(HDC & hdc);

	void LoadRunwayData();
	void LoadRunwaysFromSectorfile();
	void LoadRunwayUserSettings(CRunway & runway);

	bool IsDataUpdated() const;
	void DisplayMessage(std::string message);
};

