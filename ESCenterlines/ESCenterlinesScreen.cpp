#include "ESCenterlinesScreen.h"

CESCenterlinesScreen::CESCenterlinesScreen(FILETIME & sTime, CCenterlineSettings & centerline_settings)
	: ActiveRunwaysUpdateTime(sTime), centerline_settings(centerline_settings)
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
		RefreshActiveRunways();
}

void CESCenterlinesScreen::OnAsrContentLoaded(bool Loaded)
{
	InitAsrSettings();
	LoadRunwayData();
	RefreshActiveRunways();
}

void CESCenterlinesScreen::OnAsrContentToBeSaved(void)
{
	SaveDataToAsr(DISPLAY_CENTERLINES, DISPLAY_CENTERLINES, display_centerlines ? "1" : "0");
	SaveDataToAsr(DISPLAY_ACTIVE, DISPLAY_ACTIVE, display_active ? "1" : "0");
}

bool CESCenterlinesScreen::OnCompileCommand(const char * sCommandLine)
{
	if (!strcmp(sCommandLine, ".cline active"))
	{
		display_active = !display_active;
		RefreshMapContent();
		return true;
	}
	if (!strcmp(sCommandLine, ".cline display"))
	{
		display_centerlines = !display_centerlines;
		RefreshMapContent();
		return true;
	}
	if (!strcmp(sCommandLine, ".cline save"))
	{
		centerline_settings.Save(runways);
		return true;
	}
	return false;
}

void CESCenterlinesScreen::DrawExtendedCenterlines(HDC & hDC)
{
	if (!display_centerlines)
		return;
	auto pen = CreatePen(BS_SOLID, 1, RGB(200, 200, 200));
	auto old_pen = SelectObject(hDC, pen);
	for (auto & runway : runways)
		if (!display_active || IsRunwayActive(runway->identifier))
			DrawLines(hDC, runway->GetLines());
	SelectObject(hDC, old_pen);
}

void CESCenterlinesScreen::DrawLines(HDC & hDC, const std::vector<CLine>& lines)
{
	for (auto & line : lines)
	{
		if (line.depending && (!display_active || IsRunwayActive(line.depends_on)))
			continue;
		auto pp1 = ConvertCoordFromPositionToPixel(line.c1);
		auto pp2 = ConvertCoordFromPositionToPixel(line.c2);
		MoveToEx(hDC, pp1.x, pp1.y, nullptr);
		LineTo(hDC, pp2.x, pp2.y);
	}
}

std::unique_ptr<CCoordinate> CESCenterlinesScreen::GetFixCoordinate(const std::string & name, const CCoordinate & threshold)
{
	if (!name.length())
		return nullptr;
	for (auto se = GetPlugIn()->SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_FIX);
		 se.IsValid();
		 se = GetPlugIn()->SectorFileElementSelectNext(se, EuroScopePlugIn::SECTOR_ELEMENT_FIX))
	{
		if (se.GetName() == name)
		{
			CCoordinate c;
			se.GetPosition(&c, 0);
			if (c.DistanceTo(threshold) <= 30)
				return std::make_unique<CCoordinate>(c);
		}
	}
	return nullptr;
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

bool CESCenterlinesScreen::IsRunwayActive(const Identifier & identifier) const
{
	for (auto & id : active_runways)
		if (id == identifier)
			return true;
	return false;
}

void CESCenterlinesScreen::LoadRunwayData()
{
	for (auto se = GetPlugIn()->SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
		 se.IsValid();
		 se = GetPlugIn()->SectorFileElementSelectNext(se, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
	{
		for (auto i = 0; i < 2; ++i)
		{
			std::unique_ptr<CRunway> runway = CRunway::CreateRunway(se, i);
			if (runway)
			{
				centerline_settings.GetCenterlineSettings(*runway);
				geographic.CalculateExtendedCenterline(*runway, GetFixCoordinate(runway->extended_centerline.final_approach_fix, runway->threshold).get());
				centerline_settings.RemoveFap(runway->identifier, runway->calculated_approach_course);
				runways.push_back(std::move(runway));
			}
		}
	}
}

void CESCenterlinesScreen::RefreshActiveRunways()
{
	ActiveRunwaysLastUpdateTime = ActiveRunwaysUpdateTime; // Set timestamp to last update
	active_runways.clear();
	for (auto se = GetPlugIn()->SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
		 se.IsValid();
		 se = GetPlugIn()->SectorFileElementSelectNext(se, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
	{
		for (auto i = 0; i < 2; ++i)
			if (se.IsElementActive(false, i))
			{
				std::string airport = se.GetAirportName();
				airport.resize(4);
				std::string runway = se.GetRunwayName(i);
				active_runways.push_back(Identifier(airport, runway));
			}	
	}
	RefreshMapContent();
}

inline bool CESCenterlinesScreen::IsDataUpdated() const
{
	return ActiveRunwaysUpdateTime.dwLowDateTime != ActiveRunwaysLastUpdateTime.dwLowDateTime
		|| ActiveRunwaysUpdateTime.dwHighDateTime != ActiveRunwaysLastUpdateTime.dwHighDateTime;
}

inline void CESCenterlinesScreen::DisplayMessage(std::string message)
{
	GetPlugIn()->DisplayUserMessage("Message", "ES Centerlines", message.c_str(), true, true, false, false, false);
}
