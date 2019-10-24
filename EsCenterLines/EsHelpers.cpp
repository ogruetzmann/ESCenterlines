#include "EsHelpers.h"
#pragma warning (disable : 4172)

EsIterator::EsIterator(SectorContainer *sc, EuroScopePlugIn::CSectorElement element)
	:container(sc), sector_element(element)
{
}

void EsIterator::operator++()
{
	sector_element = container->next();
}

bool EsIterator::operator!=(EsIterator &other)
{
	if (sector_element.IsValid())
		return true;
	return false;
}

EuroScopePlugIn::CSectorElement &EsIterator::operator*()
{
	return sector_element;
}

SectorContainer::SectorContainer(EuroScopePlugIn::CPlugIn *plugin, int element_type)
: plugin(plugin), element_type(element_type)
{
}

const EsIterator &SectorContainer::begin()
{
	plugin->SelectActiveSectorfile();
	current_element = plugin->SectorFileElementSelectFirst(element_type);
	return EsIterator(this, current_element);
}

const EsIterator &SectorContainer::end()
{
	return EsIterator(this, current_element);
}

EuroScopePlugIn::CSectorElement SectorContainer::next()
{
		return (current_element = plugin->SectorFileElementSelectNext(current_element, element_type));
}

EuroScopePlugIn::CSectorElement SectorContainer::get()
{
	return current_element;
}

