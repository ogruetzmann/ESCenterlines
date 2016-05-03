#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ExtendedCenterline.h"

class CCenterlineSettings
{
public:
	struct identifier
	{
		std::string airport_designator;
		std::string runway_designator;
	};
	typedef std::pair<identifier, CExtendedCenterline> ecl_object;

	CCenterlineSettings();
	virtual ~CCenterlineSettings();

	const CExtendedCenterline & Find(const std::string airport, const std::string runway);

private:
	CExtendedCenterline default_centerline;

	std::vector<ecl_object> memory;


};

