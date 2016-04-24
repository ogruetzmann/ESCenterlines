#pragma once
#include <Windows.h>
#include "EuroScopePlugIn.h"


class CMyPosition :
	public EuroScopePlugIn::CPosition
{
public:
	CMyPosition();
	CMyPosition(double lat, double lon);
	virtual ~CMyPosition();
};

