#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <exception>
#include "ExtendedCenterline.h"
#include "Runway.h"
#include "Identifier.h"
#include "DataObjects.h"
#include "json\json.h"
#include "json\json-forwards.h"

const char FILENAME[] = "clsettings.cfg";

class CCenterlineSettings
{
public:
	CCenterlineSettings();
	virtual ~CCenterlineSettings();

	CExtendedCenterline* GetExtendedCenterline(const Identifier& id);
	void Save(std::vector<std::unique_ptr<CRunway>> & runways);

private:
	const std::string AIRPORT = "airport";
	const std::string RUNWAY = "runway";
	const std::string COURSE = "course";
	const std::string FAP = "fap";
	const std::string DIST_THR = "dist_thr";
	const std::string DIST_CL = "dist_cl";
	const std::string LENGTH = "length";
	const std::string ANGLE = "angle";
	const std::string DEPENDS_APT = "d_airport";
	const std::string DEPENDS_RWY = "d_runway";
	const std::string DASH = "dash";
	const std::string GAP = "gap";
	const std::string STARTS_GAP = "starts_gap";
	const std::string MARKERS = "cl_markers";
	const std::string ELEMENTS = "cl_elements";
	const std::string DIRECTION = "direction";
	const std::string NUMBER = "number";

	Identifier default_identifier { "*", "*" };
	CExtendedCenterline default_centerline;
	std::vector<std::unique_ptr<CExtendedCenterline>> memory;
	std::vector<std::string> file_errors;

	void Load();
	void Save();
	Identifier GetId(Json::Value& j);
	std::vector<CenterlineElement> GetElements(Json::Value& j);
	std::vector<CenterlineMarker> GetMarkers(Json::Value& j);
	//Json::Value ConvertToJson(const std::vector<CRangeTick> &rt);
	//Json::Value ConvertToJson(const std::vector<CCenterlineElement> &rt);
	//bool ReadFromJson(const Json::Value & jv, std::vector<CRangeTick> & rt);
	//bool ReadFromJson(const Json::Value & jv, std::vector<CCenterlineElement> & ce);
	void WriteToFile(Json::Value & j, std::string filename = FILENAME);
	void LoadFromFile(Json::Value & j);
};

