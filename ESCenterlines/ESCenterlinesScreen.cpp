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
	InitAsrSettings();
	RefreshData();
	RefreshMapContent();
}

bool CESCenterlinesScreen::OnCompileCommand(const char * sCommandLine)
{
	if (!strcmp(sCommandLine, ".cline active"))
	{
		display_active = !display_active;
		RefreshData();
		RefreshMapContent();
		return true;
	}
	if (!strcmp(sCommandLine, ".cline display"))
	{
		display_centerlines = !display_centerlines;
		RefreshData();
		RefreshMapContent();
		return true;
	}
	return false;
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
	auto pos = 0.0;
	for (auto & cl : runway.extended_centerline.centerline_elements)
	{
		auto pattern_length = cl.tick_length + cl.gap_length;
		for (auto i = 0; i < cl.length; ++i)
		{
			auto line_start = i * pattern_length + pos;
			if (cl.starts_with_gap)
				line_start += cl.gap_length;
			auto line_end = line_start + cl.tick_length;
			
			auto c1 = geographic.GetCoordinate(runway.threshold, runway.GetApproachCourse(), -(InNM(line_start)));
			auto c2 = geographic.GetCoordinate(runway.threshold, runway.GetApproachCourse(), -(InNM(line_end)));
			lines.push_back(std::make_unique<CLine>(c1, c2));
		}
		pos += pattern_length * cl.length;
	}
}

void CESCenterlinesScreen::CalculateRangeTicks(CRunway & runway)
{
	for (auto & rt : runway.extended_centerline.range_ticks)
	{
		auto tick_azimuth_left = runway.GetApproachCourse() - 90;
		auto c_base = geographic.GetCoordinate(runway.threshold, runway.GetApproachCourse(), -(InNM(rt.distance_thr)));
		if (rt.direction == Direction::left || rt.direction == Direction::both)
		{
			auto c1_left = geographic.GetCoordinate(c_base, tick_azimuth_left, InNM(rt.distance_cl));
			auto c2_left = geographic.GetCoordinate(c1_left, tick_azimuth_left, InNM(rt.length));
			lines.push_back(std::make_unique<CLine>(c1_left, c2_left));
		}
		if (rt.direction == Direction::right || rt.direction == Direction::both)
		{
			auto c1_right = geographic.GetCoordinate(c_base, tick_azimuth_left, -InNM(rt.distance_cl));
			auto c2_right = geographic.GetCoordinate(c1_right, tick_azimuth_left, -InNM(rt.length));
			lines.push_back(std::make_unique<CLine>(c1_right, c2_right));
		}
	}
}

void CESCenterlinesScreen::CreateCenterlines()
{
	lines.clear();
	if (!display_centerlines)
		return;
	for (auto & rs : runways)
	{
		if (display_active && !rs->is_active)
			continue;

		CalculateCenterline(*rs);
		CalculateRangeTicks(*rs);
	}
}

void CESCenterlinesScreen::InitAsrSettings()
{
	if (!(GetDataFromAsr(DISPLAY_CENTERLINES)))
		SaveDataToAsr(DISPLAY_CENTERLINES, DISPLAY_CENTERLINES, "1");
	if (!strcmp(GetDataFromAsr(DISPLAY_CENTERLINES), "0"))
			display_centerlines = false;
	if (!(GetDataFromAsr(DISPLAY_ACTIVE)))
		SaveDataToAsr(DISPLAY_ACTIVE, DISPLAY_ACTIVE, "1");
	if (!strcmp(GetDataFromAsr(DISPLAY_ACTIVE), "0"))
		display_active = false;
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
