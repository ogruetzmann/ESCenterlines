#include "CenterlineSettings.h"



CCenterlineSettings::CCenterlineSettings()
{
	default_centerline.SetDefault();
}


CCenterlineSettings::~CCenterlineSettings()
{
}

const CExtendedCenterline & CCenterlineSettings::Find(const std::string airport, const std::string runway)
{
	for (auto & m : memory)
	{
		if (m.first.airport_designator == airport && m.first.runway_designator == runway)
			return m.second;
	}
	return default_centerline;
}
