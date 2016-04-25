#include "ESCenterlinesScreen.h"

CESCenterlinesScreen::CESCenterlinesScreen(FILETIME & sTime) : ActiveRunwaysUpdateTime(sTime)
{
	ActiveRunwaysLastUpdateTime = sTime;
}


CESCenterlinesScreen::~CESCenterlinesScreen()
{
}

void CESCenterlinesScreen::OnRefresh(HDC hDC, int Phase)
{
	if (Phase == EuroScopePlugIn::REFRESH_PHASE_BACK_BITMAP)
		DrawExtendedCenterlines(hDC);


	if (IsDataUpdated())
	{
		LoadRunwayData();
		ActiveRunwaysLastUpdateTime = ActiveRunwaysUpdateTime; // Set timestamp to last update
		RefreshMapContent();
	}
}

void CESCenterlinesScreen::OnAsrContentLoaded(bool Loaded)
{
	LoadRunwayData();
}

void CESCenterlinesScreen::DrawExtendedCenterlines(HDC & hDC)
{
	/*HGDIOBJ pen = CreatePen(BS_SOLID, 1, RGB(200, 200, 200));
	HGDIOBJ old_pen = SelectObject(hDC, pen);
	MoveToEx(hDC, 100, 100, 0);
	LineTo(hDC, 200, 200);
	SelectObject(hDC, old_pen);*/
}

void CESCenterlinesScreen::LoadRunwayData()
{
	for (EuroScopePlugIn::CSectorElement se = GetPlugIn()->SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
		 se.IsValid();
		 se = GetPlugIn()->SectorFileElementSelectNext(se, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
	{
		CRunway runway1(se, 0);
		LoadRunwayUserSettings(runway1);
		CRunway runway2(se, 1);
		LoadRunwayUserSettings(runway2);

		// ToDo: Save data elsewhere
	}
}

void CESCenterlinesScreen::LoadRunwayUserSettings(CRunway & runway)
{
}

bool CESCenterlinesScreen::IsDataUpdated() const
{
	if (ActiveRunwaysUpdateTime.dwLowDateTime != ActiveRunwaysLastUpdateTime.dwLowDateTime
		|| ActiveRunwaysUpdateTime.dwHighDateTime != ActiveRunwaysLastUpdateTime.dwHighDateTime)
		return true;
	return false;
}

void CESCenterlinesScreen::DisplayMessage(std::string message)
{
	GetPlugIn()->DisplayUserMessage("ES Centerlines", "Debug", message.c_str(), true, true, false, false, false);
}
