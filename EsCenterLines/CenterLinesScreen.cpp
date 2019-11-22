#include "CenterLinesScreen.h"

CenterLinesScreen::CenterLinesScreen(std::list<CLine> &lines, std::list<CLine> &ticks, std::list<CenterLinesScreen *> &screens)
	:lines(lines), ticks(ticks), screens(screens)
{
	auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	hr = pD2DFactory->CreateDCRenderTarget(&rt_properties, &render_target);
	D2D1_COLOR_F clcolor;
	clcolor.r = float(180) / float(255);
	clcolor.g = float(180) / float(255);
	clcolor.b = float(180) / float(255);
	clcolor.a = 1;
	render_target->CreateSolidColorBrush(clcolor, &pBlackBrush);
}

void CenterLinesScreen::OnAsrContentLoaded(bool loaded)
{
	if (!loaded)
		mode = Plugin_Mode::off;
	else
	{
		auto str = GetDataFromAsr("Active");
		if (str == nullptr)
			mode = Plugin_Mode::off;
		else
		{
			if (!strcmp("0", str))
				mode = Plugin_Mode::off;
			else if (!strcmp("1", str))
				mode = Plugin_Mode::runway;
			else if (!strcmp("2", str))
				mode = Plugin_Mode::airport;
			else if (!strcmp("3", str))
				mode = Plugin_Mode::on;
		}
	}
	RefreshMapContent();
}

void CenterLinesScreen::OnAsrContentToBeClosed()
{
	screens.remove(this);
	delete this;
}

void CenterLinesScreen::OnRefresh(HDC hDC, int phase)
{
	if (phase == EuroScopePlugIn::REFRESH_PHASE_BACK_BITMAP)
	{
		DrawCenterlines(hDC);
	}
}

bool CenterLinesScreen::OnCompileCommand(const char *sCommandLine)
{
	if (!strcmp(".esclines", sCommandLine))
	{
		//menu
		return false;
	}
	if (!strcmp(".esclines off", sCommandLine))
	{
		SaveDataToAsr("Active", "", "0");
		mode = Plugin_Mode::off;
		RefreshMapContent();
		return true;
	}
	if (!strcmp(".esclines rwy", sCommandLine))
	{
		SaveDataToAsr("Active", "", "1");
		mode = Plugin_Mode::runway;
		RefreshMapContent();
		return true;
	}
	if (!strcmp(".esclines apt", sCommandLine))
	{
		SaveDataToAsr("Active", "", "2");
		mode = Plugin_Mode::airport;
		RefreshMapContent();
		return true;
	}
	if (!strcmp(".esclines on", sCommandLine))
	{
		SaveDataToAsr("Active", "", "3");
		mode = Plugin_Mode::on;
		RefreshMapContent();
		return true;
	}
	return false;
}

void CenterLinesScreen::DrawCenterlines(HDC hDC)
{
	if (mode == Plugin_Mode::off)
		return;
	
	auto rect = GetRadarArea();
	rect.top = GetToolbarArea().top;
	rect.bottom = GetChatArea().bottom;
	render_target->BindDC(hDC, &rect);
	render_target->BeginDraw();

	for (auto x : lines)
	{
		if (mode == Plugin_Mode::runway && !x.rwy || mode == Plugin_Mode::airport && !x.apt)
			continue;
		auto start = ConvertCoordFromPositionToPixel(x.start.cposition());
		auto end = ConvertCoordFromPositionToPixel(x.end.cposition());
		render_target->DrawLine({ float(start.x), float(start.y) }, { float(end.x), float(end.y) }, pBlackBrush, 1);
	}

	for (auto x : ticks)
	{
		if (mode == Plugin_Mode::runway && !x.rwy || mode == Plugin_Mode::airport && !x.apt)
			continue;
		auto start = ConvertCoordFromPositionToPixel(x.start.cposition());
		auto end = ConvertCoordFromPositionToPixel(x.end.cposition());
		render_target->DrawLine({ float(start.x), float(start.y) }, { float(end.x), float(end.y) }, pBlackBrush, 1);
	}
	render_target->EndDraw();
}
