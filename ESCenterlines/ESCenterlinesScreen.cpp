#include "ESCenterlinesScreen.h"

CESCenterlinesScreen::CESCenterlinesScreen(FILETIME & sTime) : ActiveRunwaysUpdateTime(sTime)
{
	ActiveRunwaysLastUpdateTime = sTime;
}


CESCenterlinesScreen::~CESCenterlinesScreen()
{
	if (Runways)
	{
		Runways->clear();
		delete Runways;
	}
}

void CESCenterlinesScreen::OnRefresh(HDC hDC, int Phase)
{
	bool runwayUpdate = false;
	// Check if the runway configuration was updated
	if (ActiveRunwaysUpdateTime.dwLowDateTime != ActiveRunwaysLastUpdateTime.dwLowDateTime
		|| ActiveRunwaysUpdateTime.dwHighDateTime != ActiveRunwaysLastUpdateTime.dwHighDateTime)
	{
		runwayUpdate = true;
		ActiveRunwaysLastUpdateTime = ActiveRunwaysUpdateTime;
	}

	std::string activeonly = GetPlugIn()->GetDataFromSettings("m_ExtendedCenterlinActiveArr");

	if (Phase == EuroScopePlugIn::REFRESH_PHASE_BACK_BITMAP)
	{
		if (!Runways)
		{
			return;
		}

		bool displayAll = false;
		HGDIOBJ Pen = CreatePen(PS_SOLID, 1, RGB(150,150,150));
		HGDIOBJ oldPen = SelectObject(hDC, Pen);
		LPPOINT pt = 0;
		GeographicLib::Constants::nauticalmile();
		
		for (CRunway &rwy : *Runways)
		{
			POINT start1, start2, end1, end2;
			static GeographicLib::Geodesic g(GeographicLib::Geodesic::WGS84());
			EuroScopePlugIn::CPosition pos;

			if (rwy.GetActive(0) || displayAll)
			{
				rwy.GetThreshold(pos, 0);
				start1 = ConvertCoordFromPositionToPixel(pos);
				g.Direct(pos.m_Latitude, pos.m_Longitude, rwy.GetHeading(0), -20 * GeographicLib::Constants::nauticalmile(), pos.m_Latitude, pos.m_Longitude);
				end1 = ConvertCoordFromPositionToPixel(pos);

				MoveToEx(hDC, start1.x, start1.y, pt);
				LineTo(hDC, end1.x, end1.y);
			}

			if (rwy.GetActive(1) || displayAll)
			{
				rwy.GetThreshold(pos, 1);
				start2 = ConvertCoordFromPositionToPixel(pos);
				g.Direct(pos.m_Latitude, pos.m_Longitude, rwy.GetHeading(1), -20 * GeographicLib::Constants::nauticalmile(), pos.m_Latitude, pos.m_Longitude);
				end2 = ConvertCoordFromPositionToPixel(pos);

				MoveToEx(hDC, start2.x, start2.y, pt);
				LineTo(hDC, end2.x, end2.y);
			}
		}
		SelectObject(hDC, oldPen);
	}

	if (runwayUpdate)
	{
		LoadRunways();
	}
}

void CESCenterlinesScreen::OnAsrContentLoaded(bool Loaded)
{
	LoadRunways();
}

void CESCenterlinesScreen::LoadRunways()
{
	if (!Runways)
	{
		if (!(Runways = new std::vector<CRunway>))
		{
			return;
		}
	}
	Runways->clear();

	for (EuroScopePlugIn::CSectorElement se = GetPlugIn()->SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);
		 se.IsValid();
		 se = GetPlugIn()->SectorFileElementSelectNext(se, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
	{
		CRunway rwy;
		EuroScopePlugIn::CPosition position;

		rwy.SetAirportName(std::string(se.GetAirportName()));
		for (int i = 0; i < 2; ++i)
		{
			se.GetPosition(&position, i);
			rwy.SetThreshold(position, i);
			rwy.SetDesignator(se.GetRunwayName(i), i);
			rwy.SetActive(se.IsElementActive(false, i), i);
		}
		Runways->push_back(rwy);
	}

	RefreshMapContent();
}
