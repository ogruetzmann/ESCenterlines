#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <exception>
#include "ExtendedCenterline.h"
#include "Runway.h"
#include "DataObjects.h"
#include "json\json.h"
#include "json\json-forwards.h"

const char FILENAME[] = "clsettings.cfg";

class CCenterlineSettings
{
public:
	typedef std::pair<Identifier, CExtendedCenterline> ecl_object;

	CCenterlineSettings();
	virtual ~CCenterlineSettings();

	CExtendedCenterline& GetExtendedCenterline(const Identifier& id) const;
	int Errors() const;

	void RemoveFap(Identifier &id, double course);
	std::vector<std::string> GetErrorStrings() const;
	void Save(std::vector<std::unique_ptr<CRunway>> & runways);

private:
	std::unique_ptr<CExtendedCenterline> default_centerline { nullptr };
	std::vector<std::unique_ptr<ecl_object>> memory;
	std::vector<std::string> file_errors;

	void Load();
	void Save();
	//Json::Value ConvertToJson(const std::vector<CRangeTick> &rt);
	//Json::Value ConvertToJson(const std::vector<CCenterlineElement> &rt);
	//bool ReadFromJson(const Json::Value & jv, std::vector<CRangeTick> & rt);
	//bool ReadFromJson(const Json::Value & jv, std::vector<CCenterlineElement> & ce);
	void WriteToFile(Json::Value & j, std::string filename = FILENAME);
	void LoadFromFile(Json::Value & j);
};

