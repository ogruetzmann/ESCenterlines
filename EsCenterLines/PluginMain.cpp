#include "EuroScopePlugIn.h"
#include "EsCenterLines.h"

EuroScopePlugIn::CPlugIn *pPlugin{ nullptr };

void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn **ppPlugInInstance)
{
	*ppPlugInInstance = pPlugin = new EsCenterLines();
}

void __declspec (dllexport) EuroScopePlugInExit()
{
	delete pPlugin;
}