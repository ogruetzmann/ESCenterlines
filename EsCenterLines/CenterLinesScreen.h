#pragma once
#pragma comment(lib, "d2d1")
#include <Windows.h>
#include <d2d1.h>
#include <list>
#include <EuroScopePlugIn.h>
#include "DrawResources.h"
#include "Runway.h"
class CenterLinesScreen :
	public EuroScopePlugIn::CRadarScreen
{
public:
	CenterLinesScreen(std::list<Runway> &runways);
private:
	std::list<Runway> &runways;
	ID2D1Factory *pD2DFactory{ nullptr };
	void OnAsrContentToBeClosed();
	void OnRefresh(HDC hDC, int phase);

	void DrawRectangle(HDC hDC);
	void DrawCenterlines(HDC hDC);
	
};

