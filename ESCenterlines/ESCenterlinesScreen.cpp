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
		RefreshMapContent();
	}
}

void CESCenterlinesScreen::OnAsrContentLoaded(bool Loaded)
{
	RefreshData();
	RefreshMapContent();
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

void CESCenterlinesScreen::CalculateCenterline(CRunway & runway)
{
	double pos = 0.0;
	for (auto & cl : runway.extended_centerline.centerline_elements)
	{
		auto pattern_length = cl.tick_length + cl.gap_length;
		for (auto i = 0; i < cl.length; ++i)
		{
			auto line_start = i * pattern_length + pos;
			if (cl.starts_with_gap)
				line_start += cl.gap_length;
			auto line_end = line_start + cl.tick_length;
			
			CCoordinate c1 = geographic.GetCoordinate(runway.threshold, runway.GetApproachCourse(), -(InNM(line_start)));
			CCoordinate c2 = geographic.GetCoordinate(runway.threshold, runway.GetApproachCourse(), -(InNM(line_end)));
			lines.push_back(std::make_unique<CLine>(c1, c2));
		}
		pos += pattern_length * cl.length;
	}
}

void CESCenterlinesScreen::CalculateRangeTicks(CRunway & runway)
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
	ActiveRunwaysLastUpdateTime = ActiveRunwaysUpdateTime; // Set timestamp to last update
}

inline bool CESCenterlinesScreen::IsDataUpdated() const
{
	return (ActiveRunwaysUpdateTime.dwLowDateTime != ActiveRunwaysLastUpdateTime.dwLowDateTime
			|| ActiveRunwaysUpdateTime.dwHighDateTime != ActiveRunwaysLastUpdateTime.dwHighDateTime);
}

inline void CESCenterlinesScreen::DisplayMessage(std::string message)
{
	GetPlugIn()->DisplayUserMessage("Message", "ES Centerlines", message.c_str(), true, true, false, false, false);
}
