#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ExtendedCenterline.h"
#include "Runway.h"
#include "DataObjects.h"
#include "json\json.h"
#include "json\json-forwards.h"

class CCenterlineSettings
{
public:
	typedef std::pair<identifier, CExtendedCenterline> ecl_object;

	CCenterlineSettings();
	virtual ~CCenterlineSettings();

	void GetCenterlineSettings(CRunway & runway);

private:
	CExtendedCenterline default_centerline;
	std::vector<ecl_object> memory;

	void Load();
	void Save();
};

