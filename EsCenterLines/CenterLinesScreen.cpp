#include "CenterLinesScreen.h"

CenterLinesScreen::CenterLinesScreen(std::list<CLine> &lines, std::list<CLine> &ticks)
	:lines(lines), ticks(ticks)
{
	auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	hr = pD2DFactory->CreateDCRenderTarget(&rt_properties, &render_target);
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &pBlackBrush);
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &pGreenBrush);
}

void CenterLinesScreen::OnAsrContentLoaded(bool loaded)
{
}

void CenterLinesScreen::OnAsrContentToBeClosed()
{
	delete this;
}

void CenterLinesScreen::OnRefresh(HDC hDC, int phase)
{
	if (phase == EuroScopePlugIn::REFRESH_PHASE_BACK_BITMAP)
	{
		DrawRectangle(hDC);
		DrawCenterlines(hDC);
	}
}

void CenterLinesScreen::DrawRectangle(HDC hDC)
{
	auto rect = GetRadarArea();
	rect.top = GetToolbarArea().top;
	rect.bottom = GetChatArea().bottom;
	render_target->BindDC(hDC, &rect);
	render_target->BeginDraw();

	for (auto x : lines)
	{
		auto start = ConvertCoordFromPositionToPixel(x.start.cposition());
		auto end = ConvertCoordFromPositionToPixel(x.end.cposition());
		render_target->DrawLine({ float(start.x), float(start.y) }, { float(end.x), float(end.y) }, pBlackBrush, 1);
	}

	for (auto x : ticks)
	{
		auto start = ConvertCoordFromPositionToPixel(x.start.cposition());
		auto end = ConvertCoordFromPositionToPixel(x.end.cposition());
		render_target->DrawLine({ float(start.x), float(start.y) }, { float(end.x), float(end.y) }, pBlackBrush, 1);
	}
	render_target->EndDraw();
}

void CenterLinesScreen::DrawCenterlines(HDC hDC)
{
}
