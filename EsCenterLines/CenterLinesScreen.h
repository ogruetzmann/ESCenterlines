#pragma once
#pragma comment(lib, "d2d1")
#include <Windows.h>
#include <d2d1.h>
#include <list>
#include <EuroScopePlugIn.h>
#include "Runway.h"
class CenterLinesScreen :
	public EuroScopePlugIn::CRadarScreen
{
public:
	CenterLinesScreen(std::list<CLine> &lines, std::list<CLine> &ticks);
private:
	D2D1_RENDER_TARGET_PROPERTIES rt_properties = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);
	ID2D1Factory *pD2DFactory{ nullptr };
	ID2D1DCRenderTarget *render_target{ nullptr };
	ID2D1SolidColorBrush *pBlackBrush{ nullptr };
	ID2D1SolidColorBrush *pRedBrush{ nullptr };
	ID2D1SolidColorBrush *pGreenBrush{ nullptr };
	std::list<CLine> &lines;
	std::list<CLine> &ticks;

	void OnAsrContentLoaded(bool loaded);
	void OnAsrContentToBeClosed();
	void OnRefresh(HDC hDC, int phase);

	void DrawRectangle(HDC hDC);
	void DrawCenterlines(HDC hDC);
	
};

