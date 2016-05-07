#include "CenterlineSettings.h"



CCenterlineSettings::CCenterlineSettings()
{
	Load();
	SetDefault();

}


CCenterlineSettings::~CCenterlineSettings()
{
	Save();
}

CExtendedCenterline* CCenterlineSettings::GetExtendedCenterline(const Identifier& id)
{
	for (auto & m : memory)
		if (m->GetIdentifier() == id)
			return m.get();
	return default_centerline.get();
}

void CCenterlineSettings::Reload()
{
	default_centerline.release();
	memory.clear();
	Load();
	SetDefault();
}

void CCenterlineSettings::Load()
{
	std::unique_ptr<Json::Value> j_runways = std::make_unique<Json::Value>(Json::arrayValue);
	LoadFromFile(*j_runways);
	for (auto & j : *j_runways)
	{
		bool default_line { false };
		Identifier id = GetId(j);
		std::unique_ptr<CExtendedCenterline> centerline;
		CExtendedCenterline* runway { nullptr };
		if (!id.size())
			continue;
		if (id == default_identifier)
		{
			default_line = true;
			default_centerline = std::make_unique<CExtendedCenterline>(id);
			runway = default_centerline.get();
		}
		else
		{
			centerline = std::make_unique<CExtendedCenterline>(id);
			runway = centerline.get();
		}
		Json::Value jval;
		if ((jval = j.get(COURSE, 0)).isDouble())
			runway->SetCourse(jval.asDouble());
		if ((jval = j.get(FAP, "")).isString())
			runway->SetFinalApproachFix(jval.asString());

		if ((jval = j.get(MARKERS, Json::nullValue)).isArray())
		{
			std::vector<CenterlineMarker> clm;
			clm = GetMarkers(jval);
			for (auto &m : clm)
				runway->AddMarker(std::move(m));
		}
		if ((jval = j.get(ELEMENTS, Json::nullValue)).isArray())
		{
			std::vector<CenterlineElement> cle;
			cle = GetElements(jval);
			for (auto &e : cle)
				runway->AddElement(std::move(e));
		}

		if (centerline)
			memory.push_back(std::move(centerline));
	}
}

void CCenterlineSettings::Save()
{
	//for (auto & m : memory)
	//{

	//}
}

Identifier CCenterlineSettings::GetId(Json::Value & j)
{
	Json::Value jval;
	std::string airport_name {}, runway_name {};
	if ((jval = j.get(AIRPORT, "")).isString())
		airport_name = j.get(AIRPORT, "").asString();
	if ((jval = j.get(RUNWAY, "")).isString())
		runway_name = j.get(RUNWAY, "").asString();
	Identifier id(airport_name, runway_name);
	return id;
}

std::vector<CenterlineElement> CCenterlineSettings::GetElements(Json::Value & j_arr)
{
	std::vector<CenterlineElement> cle;
	for (auto &j : j_arr)
	{
		Json::Value jval;
		double dash { 0 };
		double gap { 0 };
		int number { 0 };
		bool starts_gap { true };
		
		if ((jval = j.get(DASH, 0.0)).isDouble())
			dash = jval.asDouble();
		if ((jval = j.get(GAP, 0.0)).isDouble())
			gap = jval.asDouble();
		if ((jval = j.get(NUMBER, 0.0)).isInt())
			number = jval.asInt();
		if ((jval = j.get(STARTS_GAP, 0.0)).isBool())
			starts_gap = jval.asBool();
		cle.push_back(CenterlineElement { dash, gap, number, starts_gap });
	}
	return cle;
}

std::vector<CenterlineMarker> CCenterlineSettings::GetMarkers(Json::Value & j_arr)
{
	std::vector<CenterlineMarker> clm;
	for (auto &j : j_arr)
	{
		Json::Value jval;
		double angle { 0 };
		double dist_cl { 0 };
		double dist_thr { 0 };
		double length { 0 };
		Direction direction { Direction::both };
		std::string depends_airport {};
		std::string depends_runway {};

		if ((jval = j.get(ANGLE, 0.0)).isDouble())
			angle = jval.asDouble();
		if ((jval = j.get(DIST_CL, 0.0)).isDouble())
			dist_cl = jval.asDouble();
		if ((jval = j.get(DIST_THR, 0.0)).isDouble())
			dist_thr = jval.asDouble();
		if ((jval = j.get(LENGTH, 0.0)).isDouble())
			length = jval.asDouble();
		if ((jval = j.get(DIRECTION, 0)).isInt())
		{
			auto i = jval.asInt();
			if (i >= 0 && i <= 2)
				direction = Direction(i);
			else
				direction = Direction::both;
		}
		if ((jval = j.get(DEPENDS_APT, "")).isString())
			depends_airport = jval.asString();
		if ((jval = j.get(DEPENDS_RWY, "")).isString())
			depends_runway = jval.asString();

		if (!(depends_airport.length() + depends_runway.length()))
			clm.push_back(CenterlineMarker { angle, dist_thr, dist_cl, length, direction, nullptr });
		else
			clm.push_back(CenterlineMarker { angle, dist_thr, dist_cl, length, direction, std::make_unique<Identifier>(depends_airport, depends_runway) });
	}
	return clm;
}

void CCenterlineSettings::SetDefault()
{
	if (!default_centerline)
	{
		default_centerline = std::make_unique<CExtendedCenterline>(default_identifier);
		default_centerline->AddElement(CenterlineElement { 1, 1, 10, true });
		default_centerline->AddMarker(CenterlineMarker { 0, 4, 0.5, 0.5, Direction::both, nullptr });
		default_centerline->AddMarker(CenterlineMarker { 0, 10, 0.5, 0.5, Direction::both, nullptr });
		default_centerline->AddMarker(CenterlineMarker { 0, 20, 0.5, 0.5, Direction::both, nullptr });
	}
}

void CCenterlineSettings::Save(std::vector<std::unique_ptr<CRunway>>& runways)
{
	//std::unique_ptr<Json::Value> j_runways = std::make_unique<Json::Value>(Json::arrayValue);//(Json::arrayValue);
	//for (auto & r : runways)
	//{
	//	Json::Value runway(Json::objectValue);
	//	runway["airport"] = r->identifier.airport_designator;
	//	runway["runway"] = r->identifier.runway_designator;
	//	runway["fap"] = "";
	//	runway["course"] = r->calculated_approach_course;
	//	runway["cl_elements"] = ConvertToJson(r->extended_centerline.centerline_elements);
	//	runway["cl_markers"] = ConvertToJson(r->extended_centerline.range_ticks);
	//	j_runways->append(runway);
	//}
	//WriteToFile(*j_runways, "cloutput.txt");
}

//Json::Value CCenterlineSettings::ConvertToJson(const std::vector<CRangeTick>& rt)
//{
//	Json::Value j_markers(Json::arrayValue);
//	for (auto & x : rt)
//	{
//		Json::Value marker(Json::objectValue);
//		marker["angle"] = x.angle;
//		marker["d_airport"] = x.depends_on.airport_designator;
//		marker["d_runway"] = x.depends_on.runway_designator;
//		marker["direction"] = int(x.direction);
//		marker["dist_cl"] = x.distance_cl;
//		marker["dist_thr"] = x.distance_thr;
//		marker["length"] = x.length;
//		j_markers.append(marker);
//	}
//	return j_markers;
//}
//
//Json::Value CCenterlineSettings::ConvertToJson(const std::vector<CCenterlineElement>& ce)
//{
//	Json::Value j_elements(Json::arrayValue);
//	for (auto & x : ce)
//	{
//		Json::Value element(Json::objectValue);
//		element["gap"] = x.gap_length;
//		element["number"] = x.length;
//		element["dash"] = x.tick_length;
//		element["starts_gap"] = x.starts_with_gap;
//		j_elements.append(element);
//	}
//	return j_elements;
//}
//
//bool CCenterlineSettings::ReadFromJson(const Json::Value & jv, std::vector<CRangeTick>& rt)
//{
//	for (auto & val : jv)
//	{
//		try
//		{
//			double angle = val.get("angle", 0.0).asDouble();
//			std::string airport = val.get("d_airport", "").asString();
//			std::string runway = val.get("d_runway", "").asString();
//			Direction direction = Direction(val.get("direction", 0).asInt());
//			double dist_cl = val.get("dist_cl", 0).asDouble();
//			double dist_thr = val.get("dist_thr", 0).asDouble();
//			double length = val.get("length", 0).asDouble();
//
//			if (airport.length() && runway.length())
//				rt.push_back(CRangeTick(dist_thr, dist_cl, length, { airport, runway }, direction, angle));
//			else
//				rt.push_back(CRangeTick(dist_thr, dist_cl, length, direction, angle));
//		}
//		catch (std::exception &e)
//		{
//			file_errors.push_back(e.what());
//			return false;
//		}
//	}
//	return true;
//}
//
//bool CCenterlineSettings::ReadFromJson(const Json::Value & jv, std::vector<CCenterlineElement> & ce)
//{
//	for (auto & val : jv)
//	{
//		try
//		{
//			double dash = val.get("dash", 0).asDouble();
//			double gap = val.get("gap", 0).asDouble();
//			int number = val.get("number", 0).asInt();
//			bool starts_gap = val.get("starts_gap", false).asBool();
//			ce.push_back(CCenterlineElement(dash, gap, number, starts_gap));
//		}
//		catch (std::exception &e)
//		{
//			file_errors.push_back(e.what());
//			return false;
//		}
//	}
//	return true;
//}

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
