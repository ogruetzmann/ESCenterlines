#pragma once
#include <Windows.h>
#include "EuroScopePlugIn.h"

#define MY_PLUGIN_NAME      "ESCenterlines"
#define MY_PLUGIN_VERSION   "1.0"
#define MY_PLUGIN_DEVELOPER "Oliver Grützmann"
#define MY_PLUGIN_COPYRIGHT "Free to be distributed as source code"

class CESCenterlines :
	public EuroScopePlugIn::CPlugIn
{
public:
	CESCenterlines();
	virtual ~CESCenterlines();
};

