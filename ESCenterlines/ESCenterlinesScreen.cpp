#include "ESCenterlinesScreen.h"

CESCenterlinesScreen::CESCenterlinesScreen(FILETIME & sTime) 
	: ActiveRunwaysUpdateTime(sTime)
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
		RefreshData();
		ActiveRunwaysLastUpdateTime = ActiveRunwaysUpdateTime; // Set timestamp to last update
		RefreshMapContent();
	}
}

void CESCenterlinesScreen::OnAsrContentLoaded(bool Loaded)
{
	RefreshData();
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

void CESCenterlinesScreen::CalculateCenterline(const CRunway & runway)
{
	for (auto & cl : runway.extended_centerline.centerline_elements)
	{
		double pattern_length = cl.tick_length + cl.gap_length;
		for (auto i = 0; i < cl.length; ++i)
		{

		}
	}
}

void CESCenterlinesScreen::CalculateRangeTicks(const CRunway & runway)
{
	for (auto & rt : runway.extended_centerline.range_ticks)
	{

	}
}

void CESCenterlinesScreen::CreateCenterlines()
{
	lines.clear();
	for (auto & rs : runways)
	{
		if (!rs->is_active)
			continue;

		CalculateCenterline(*rs);
		CalculateRangeTicks(*rs);

		CCoordinate c1 { rs->threshold.m_Latitude, rs->threshold.m_Longitude };
		CCoordinate c2 { geographic.GetCoordinate(c1, rs->GetApproachCourse(CourseType::calculated), InNM(-20)) };
		lines.push_back(std::make_unique<CLine>(CLine(c1, c2)));
	}
}

void CESCenterlinesScreen::LoadRunwayData()
{
	runways.clear();
	for (auto se = GetPlugIn()->SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
		 se.IsValid();
		 se = GetPlugIn()->SectorFileElementSelectNext(se, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
	{
		for (auto i = 0; i < 2; ++i)
		{
			std::unique_ptr<CRunway> runway = CRunway::CreateRunway(se, i);
			if (runway)
				runways.push_back(std::move(runway));
		}
	}
}

void CESCenterlinesScreen::LoadRunwayUserData()
{
}

void CESCenterlinesScreen::RefreshData()
{
	LoadRunwayData();
	LoadRunwayUserData();
	CreateCenterlines();
}

inline bool CESCenterlinesScreen::IsDataUpdated() const
{
	return (ActiveRunwaysUpdateTime.dwLowDateTime != ActiveRunwaysLastUpdateTime.dwLowDateTime
			|| ActiveRunwaysUpdateTime.dwHighDateTime != ActiveRunwaysLastUpdateTime.dwHighDateTime);
}

void CESCenterlinesScreen::DisplayMessage(std::string message)
{
	GetPlugIn()->DisplayUserMessage("ES Centerlines", "Debug", message.c_str(), true, true, false, false, false);
}
