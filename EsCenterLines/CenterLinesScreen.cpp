#include "CenterLinesScreen.h"

CenterLinesScreen::CenterLinesScreen(std::list<CLine> &lines, std::list<CLine> &ticks, std::list<CenterLinesScreen *> &screens)
	:lines(lines), ticks(ticks), screens(screens)
{
	auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	hr = pD2DFactory->CreateDCRenderTarget(&rt_properties, &render_target);
	
	//clcolor.r = float(78) / float(255);
	//clcolor.g = float(62) / float(255);
	//clcolor.b = float(52) / float(255);
	clcolor.r = float(180) / float(255);
	clcolor.g = float(180) / float(255);
	clcolor.b = float(180)/ float(255);
	clcolor.a = 1;
	render_target->CreateSolidColorBrush(clcolor, &pBlackBrush);
}

void CenterLinesScreen::LoadColorSettings()
{
	int ired{ 180 }, igreen{ 180 }, iblue{ 180 };
	try
	{
		const char *red = GetPlugIn()->GetDataFromSettings("color_red");
		if (red != nullptr)
			ired = std::stoi(red);
		const char *green = GetPlugIn()->GetDataFromSettings("color_green");
		if (green != nullptr)
			igreen = std::stoi(green);
		const char *blue = GetPlugIn()->GetDataFromSettings("color_blue");
		if (blue != nullptr)
			iblue = std::stoi(blue);
	}
	catch (std::exception &e)
	{
		GetPlugIn()->DisplayUserMessage("ESCenterLines",
			"Error",
			e.what(),
			true,
			true,
			false,
			false,
			false);
		return;
	}
	clcolor.r = float(ired) / float(255);
	clcolor.g = float(igreen) / float(255);
	clcolor.b = float(iblue) / float(255);
	render_target->CreateSolidColorBrush(clcolor, &pBlackBrush);
}

void CenterLinesScreen::OnAsrContentLoaded(bool loaded)
{
	LoadColorSettings();
	if (loaded)
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
		if (!initialized)
		{
			RefreshMapContent();
			initialized = true;
		}
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
	if (!strcmp(".esclines color", sCommandLine))
	{
		ColorPicker cp;
		D2D1_COLOR_F clcolor = pBlackBrush->GetColor();
		if (cp.Picker(clcolor))
		{
			std::string r = std::to_string(int(clcolor.r * 255));
			std::string g = std::to_string(int(clcolor.g * 255));
			std::string b = std::to_string(int(clcolor.b * 255));	
			GetPlugIn()->SaveDataToSettings("color_red", "color_red", r.c_str());
			GetPlugIn()->SaveDataToSettings("color_green", "color_green", g.c_str());
			GetPlugIn()->SaveDataToSettings("color_blue", "color_blue", b.c_str());
			render_target->CreateSolidColorBrush(clcolor, &pBlackBrush);
			RefreshMapContent();
			return true;
		}
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

	for (auto &x : lines)
	{
		if (mode == Plugin_Mode::runway && !x.rwy || mode == Plugin_Mode::airport && !x.apt)
			continue;
		auto start = ConvertCoordFromPositionToPixel(x.start);
		auto end = ConvertCoordFromPositionToPixel(x.end);
		render_target->DrawLine({ float(start.x), float(start.y) }, { float(end.x), float(end.y) }, pBlackBrush, 1);
	}

	for (auto &x : ticks)
	{
		if (mode == Plugin_Mode::runway && !x.rwy || mode == Plugin_Mode::airport && !x.apt)
			continue;
		auto start = ConvertCoordFromPositionToPixel(x.start);
		auto end = ConvertCoordFromPositionToPixel(x.end);
		render_target->DrawLine({ float(start.x), float(start.y) }, { float(end.x), float(end.y) }, pBlackBrush, 1);
	}
	render_target->EndDraw();
}
