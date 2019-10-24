#include "CenterLinesScreen.h"

CenterLinesScreen::CenterLinesScreen(std::list<Runway> &runways)
	:runways(runways)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
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

	}
}

void CenterLinesScreen::DrawRectangle(HDC hDC)
{
	DrawResources dr;
	ID2D1DCRenderTarget *render_target = nullptr;
	auto rect = GetRadarArea();
	rect.top = 0;
	auto hr = pD2DFactory->CreateDCRenderTarget(&dr.rt_properties, &render_target);
	render_target->BindDC(hDC, &rect);
	ID2D1SolidColorBrush *pBlackBrush = NULL;
	if (SUCCEEDED(hr))
		hr = render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBlackBrush);
	render_target->BeginDraw();

	for (auto x : runways)
	{
		auto p_thr = ConvertCoordFromPositionToPixel(x.runway_threshold.cposition());
		auto p_end = ConvertCoordFromPositionToPixel(x.endpoint.cposition());
		render_target->DrawLine({ float(p_thr.x), float(p_thr.y) }, { float(p_end.x), float(p_end.y) }, pBlackBrush);
	}

	hr = render_target->EndDraw();
}

void CenterLinesScreen::DrawCenterlines(HDC hDC)
{
}

