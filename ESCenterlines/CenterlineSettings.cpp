#include "CenterlineSettings.h"



CCenterlineSettings::CCenterlineSettings()
{
	Load();
	default_centerline.SetDefault();
}


CCenterlineSettings::~CCenterlineSettings()
{
	Save();
}

void CCenterlineSettings::GetCenterlineSettings(CRunway & runway)
{
	for (auto & m : memory)
	{
		if (m.first.airport_designator == runway.airport_designator 
			&& m.first.runway_designator == runway.runway_designator)
		{
			runway.SetExtendedCenterline(m.second);
			return;
		}
	}
	runway.SetExtendedCenterline(default_centerline);
}

void CCenterlineSettings::Load()
{
}

void CCenterlineSettings::Save()
{
}
