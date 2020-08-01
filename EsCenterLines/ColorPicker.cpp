#include "ColorPicker.h"

ColorPicker::ColorPicker()
{
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
}

bool ColorPicker::Picker(D2D1_COLOR_F &color)
{
	cc.rgbResult = RGB(color.r * 255, color.g * 255, color.b * 255);
	if (ChooseColor(&cc))
	{
		color.r = float(GetRValue(cc.rgbResult)) / float(255);
		color.g = float(GetGValue(cc.rgbResult)) / float(255);
		color.b = float(GetBValue(cc.rgbResult)) / float(255);
		return true;
	}
	return false;
}
