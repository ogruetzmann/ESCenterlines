#include "CenterlineSettings.h"



CCenterlineSettings::CCenterlineSettings()
{
	Load();
	default_centerline.SetDefault();
}


CCenterlineSettings::~CCenterlineSettings()
{
	Save();
}

void CCenterlineSettings::GetCenterlineSettings(CRunway & runway)
{
	for (auto & m : memory)
		if (m->first == runway.identifier)
		{
			runway.SetExtendedCenterline(m->second);
			return;
		}
	runway.SetExtendedCenterline(default_centerline);
}

int CCenterlineSettings::Errors() const
{
	return file_errors.size();
}

void CCenterlineSettings::RemoveFap(Identifier & id, double course)
{
	for (auto & m : memory)
	{
		if (m->first == id)
		{
			m->second.final_approach_fix.clear();
			m->second.course = course;
		}
	}
}

std::vector<std::string> CCenterlineSettings::GetErrorStrings() const
{
	return file_errors;
}

void CCenterlineSettings::Load()
{
	std::unique_ptr<Json::Value> j_runways = std::make_unique<Json::Value>(Json::arrayValue);
	LoadFromFile(*j_runways);
	for (auto & j : *j_runways)
	{
		std::unique_ptr<ecl_object> runway = std::make_unique<ecl_object>();
		runway->first.airport_designator = j.get("airport", "").asString();
		runway->first.runway_designator = j.get("runway", "").asString();
		runway->second.final_approach_fix = j.get("fap", "").asString();
		if (j.get("course", 0).isDouble())
			runway->second.course = j.get("course", 0).asDouble();
		Json::Value markers = j.get("cl_markers", Json::nullValue);
		if (markers.isArray())
		{
			std::vector<CRangeTick> rt;
			if (ReadFromJson(markers, rt))
				for (auto & v : rt)
					runway->second.AddRangeTick(v);
			else
				continue;
		}

		Json::Value elements = j.get("cl_elements", Json::nullValue);
		if (elements.isArray())
		{
			std::vector<CCenterlineElement> ce;
			if (ReadFromJson(elements, ce))
				for (auto & v : ce)
					runway->second.AddElement(v);
			else
				continue;
		}
		memory.push_back(std::move(runway));
	}
}

void CCenterlineSettings::Save()
{
	//for (auto & m : memory)
	//{

	//}
}

void CCenterlineSettings::Save(std::vector<std::unique_ptr<CRunway>>& runways)
{
	std::unique_ptr<Json::Value> j_runways = std::make_unique<Json::Value>(Json::arrayValue);//(Json::arrayValue);
	for (auto & r : runways)
	{
		Json::Value runway(Json::objectValue);
		runway["airport"] = r->identifier.airport_designator;
		runway["runway"] = r->identifier.runway_designator;
		runway["fap"] = "";
		runway["course"] = r->calculated_approach_course;
		runway["cl_elements"] = ConvertToJson(r->extended_centerline.centerline_elements);
		runway["cl_markers"] = ConvertToJson(r->extended_centerline.range_ticks);
		j_runways->append(runway);
	}
	WriteToFile(*j_runways, "cloutput.txt");
}

Json::Value CCenterlineSettings::ConvertToJson(const std::vector<CRangeTick>& rt)
{
	Json::Value j_markers(Json::arrayValue);
	for (auto & x : rt)
	{
		Json::Value marker(Json::objectValue);
		marker["angle"] = x.angle;
		marker["d_airport"] = x.depends_on.airport_designator;
		marker["d_runway"] = x.depends_on.runway_designator;
		marker["direction"] = int(x.direction);
		marker["dist_cl"] = x.distance_cl;
		marker["dist_thr"] = x.distance_thr;
		marker["length"] = x.length;
		j_markers.append(marker);
	}
	return j_markers;
}

Json::Value CCenterlineSettings::ConvertToJson(const std::vector<CCenterlineElement>& ce)
{
	Json::Value j_elements(Json::arrayValue);
	for (auto & x : ce)
	{
		Json::Value element(Json::objectValue);
		element["gap"] = x.gap_length;
		element["number"] = x.length;
		element["dash"] = x.tick_length;
		element["starts_gap"] = x.starts_with_gap;
		j_elements.append(element);
	}
	return j_elements;
}

bool CCenterlineSettings::ReadFromJson(const Json::Value & jv, std::vector<CRangeTick>& rt)
{
	for (auto & val : jv)
	{
		try
		{
			double angle = val.get("angle", 0.0).asDouble();
			std::string airport = val.get("d_airport", "").asString();
			std::string runway = val.get("d_runway", "").asString();
			Direction direction = Direction(val.get("direction", 0).asInt());
			double dist_cl = val.get("dist_cl", 0).asDouble();
			double dist_thr = val.get("dist_thr", 0).asDouble();
			double length = val.get("length", 0).asDouble();

			if (airport.length() && runway.length())
				rt.push_back(CRangeTick(dist_thr, dist_cl, length, { airport, runway }, direction, angle));
			else
				rt.push_back(CRangeTick(dist_thr, dist_cl, length, direction, angle));
		}
		catch (std::exception &e)
		{
			file_errors.push_back(e.what());
			return false;
		}
	}
	return true;
}

bool CCenterlineSettings::ReadFromJson(const Json::Value & jv, std::vector<CCenterlineElement> & ce)
{
	for (auto & val : jv)
	{
		try
		{
			double dash = val.get("dash", 0).asDouble();
			double gap = val.get("gap", 0).asDouble();
			int number = val.get("number", 0).asInt();
			bool starts_gap = val.get("starts_gap", false).asBool();
			ce.push_back(CCenterlineElement(dash, gap, number, starts_gap));
		}
		catch (std::exception &e)
		{
			file_errors.push_back(e.what());
			return false;
		}
	}
	return true;
}

void CCenterlineSettings::WriteToFile(Json::Value & j, std::string filename)
{
	std::ofstream oFile;
	oFile.open(filename);
	if (!oFile.is_open())
		return;
	oFile << j;
	oFile.close();
}

void CCenterlineSettings::LoadFromFile(Json::Value & j)
{
	std::ifstream iFile;
	iFile.open(FILENAME);
	if (!iFile.is_open())
		return;
	try
	{
		iFile >> j;
	}
	catch (std::exception &e)
	{
		file_errors.push_back(e.what());
	}
	iFile.close();
}
