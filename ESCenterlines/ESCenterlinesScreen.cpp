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
		CreateCenterlines();
		ActiveRunwaysLastUpdateTime = ActiveRunwaysUpdateTime; // Set timestamp to last update
		RefreshMapContent();
	}
}

void CESCenterlinesScreen::OnAsrContentLoaded(bool Loaded)
{
	CreateCenterlines();
}

void CESCenterlinesScreen::DrawExtendedCenterlines(HDC & hDC)
{
	auto pen = CreatePen(BS_SOLID, 1, RGB(200, 200, 200));
	auto old_pen = SelectObject(hDC, pen);

	for (auto & line : lines)
	{
		auto pp1 = ConvertCoordFromPositionToPixel(line->c1);
		auto pp2 = ConvertCoordFromPositionToPixel(line->c2);
		MoveToEx(hDC, pp1.x, pp1.y, nullptr);
		LineTo(hDC, pp2.x, pp2.y);
	}
	SelectObject(hDC, old_pen);
}

void CESCenterlinesScreen::LoadRunwayData()
{
	runway_settings.clear();
	for (auto se = GetPlugIn()->SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
		 se.IsValid();
		 se = GetPlugIn()->SectorFileElementSelectNext(se, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
	{
		for (auto i = 0; i < 2; ++i)
		{
			std::unique_ptr<CRunway> runway = CRunway::CreateRunway(se, i);
			if (runway)
				runway_settings.push_back(std::move(runway));
		}
	}
}

void CESCenterlinesScreen::LoadRunwayUserSettings(CRunway * runway)
{
}

void CESCenterlinesScreen::CreateCenterlines()
{
	LoadRunwayData();
	LoadRunwayUserSettings(nullptr);

	lines.clear();
	static GeographicLib::Geodesic gd(GeographicLib::Geodesic::WGS84());
	for (auto & rs : runway_settings)
	{
		CCoordinate c1 { rs->threshold.m_Latitude, rs->threshold.m_Longitude };
		CCoordinate c2;

		gd.Direct(c1.m_Latitude, c1.m_Longitude, rs->GetApproachCourse(CourseType::sectorfile), InNM(-20), c2.m_Latitude, c2.m_Longitude);
		lines.push_back(std::make_unique<CLine>(CLine(c1, c2)));
	}
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
