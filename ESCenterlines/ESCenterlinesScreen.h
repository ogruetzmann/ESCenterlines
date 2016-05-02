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

	inline double InNM(double distance_in_nautical_miles)
	{
		return distance_in_nautical_miles * meters_per_nm;
	}

private:
	const FILETIME & ActiveRunwaysUpdateTime;
	FILETIME ActiveRunwaysLastUpdateTime;
	std::vector <std::unique_ptr<CRunway>> runways;
	std::vector <std::unique_ptr<CLine>> lines;
	CGeographic geographic;

	void DrawExtendedCenterlines(HDC & hdc);

	void CalculateCenterline(const CRunway & runway);
	void CalculateRangeTicks(const CRunway & runway);
	void CreateCenterlines();
	void LoadRunwayData();
	void LoadRunwayUserData();
	void RefreshData();

	bool IsDataUpdated() const;
	void DisplayMessage(std::string message);
};

