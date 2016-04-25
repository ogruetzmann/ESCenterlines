#pragma once
#include <Windows.h>
#include "EuroScopePlugIn.h"

class CCoordinate :
	public EuroScopePlugIn::CPosition
{
public:
	CCoordinate();
	CCoordinate(double latitude, double longitude);
	virtual ~CCoordinate();
};

