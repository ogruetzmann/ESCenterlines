#pragma once
#include "EuroScopePlugIn.h"

class SectorContainer;
class EsIterator
{
public:
	EsIterator(SectorContainer *sc, EuroScopePlugIn::CSectorElement element);
	~EsIterator() = default;

	void operator++ ();
	bool operator!= (EsIterator &other);
	EuroScopePlugIn::CSectorElement &operator* ();

private:
	SectorContainer *container{ nullptr };
	EuroScopePlugIn::CSectorElement sector_element;
};

class SectorContainer
{
public:
	SectorContainer(EuroScopePlugIn::CPlugIn *plugin, int element_type);
	~SectorContainer() = default;

	const EsIterator &begin();
	const EsIterator &end();
	EuroScopePlugIn::CSectorElement next();
	EuroScopePlugIn::CSectorElement get();

private:
	EuroScopePlugIn::CPlugIn *plugin{ nullptr };
	EuroScopePlugIn::CSectorElement current_element;
	int element_type{ 0 };
};

