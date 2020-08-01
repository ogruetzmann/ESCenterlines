#pragma once
#include <Windows.h>
#include <d2d1.h>
class ColorPicker
{
	CHOOSECOLOR cc;
	COLORREF acrCustClr[16];
	HWND hwnd;          
	HBRUSH hbrush;
	DWORD rgbCurrent;

public:
	ColorPicker();
	bool Picker(D2D1_COLOR_F &color);
};

