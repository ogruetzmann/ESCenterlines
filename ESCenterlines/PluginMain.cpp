#include "ESCenterlines.h"

CESCenterlines * pMyPlugIn = NULL;

void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn ** pPlugInInstance)
{
	*pPlugInInstance = pMyPlugIn = new CESCenterlines;
}

void    __declspec (dllexport)    EuroScopePlugInExit(void)
{
	delete pMyPlugIn;
}
