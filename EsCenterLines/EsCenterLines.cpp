#include "EsCenterLines.h"

const char *plugin_name{ "EsCenterLines" };
const char *plugin_version{ "0.0" };
const char *plugin_author{ "Oliver Grützmann" };
const char *plugin_license{ "tbd" };

EsCenterLines::EsCenterLines()
	: EuroScopePlugIn::CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, plugin_name, plugin_version, plugin_author, plugin_license)
{
}

EsCenterLines::~EsCenterLines()
{
}

void EsCenterLines::OnAirportRunwayActivityChanged()
{
	ReadRunways();
}

EuroScopePlugIn::CRadarScreen *EsCenterLines::OnRadarScreenCreated(const char *sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated)
{
	return nullptr;
}

void EsCenterLines::ReadRunways()
{
	std::list<Runway> runways;
	auto type = EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY;
	for (auto element = SectorFileElementSelectFirst(type); element.IsValid(); element = SectorFileElementSelectNext(element, type))
	{
		std::string airport = element.GetAirportName();
		EuroScopePlugIn::CPosition pos[2];
		if (!(element.GetPosition(&pos[0], 0) && element.GetPosition(&pos[1], 1)))
			continue;
		Coordinate threshold[2] = {{ pos[0].m_Latitude, pos[0].m_Longitude }, { pos[1].m_Latitude, pos[1].m_Longitude }};
		for (unsigned int i = 0; i < 2; ++i)
		{
			std::string runway = element.GetRunwayName(i);
			auto active_arr = element.IsElementActive(false, i);
			auto active_dep = element.IsElementActive(true, i);
			runways.push_back(Runway (std::move(airport), std::move(runway), threshold[!i ? 0 : 1], threshold[i ? 0 : 1], active_arr, active_dep));
		}
	}
}

