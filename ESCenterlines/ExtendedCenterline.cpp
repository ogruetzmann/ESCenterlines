#include "ExtendedCenterline.h"

CExtendedCenterline::CExtendedCenterline(Identifier& id)
	: id(id)
{
}

CExtendedCenterline::~CExtendedCenterline()
{
}

void CExtendedCenterline::AddElement(CenterlineElement&& e)
{
	elements.push_back(std::move(e));
}

void CExtendedCenterline::AddMarker(CenterlineMarker&& m)
{
	markers.push_back(std::move(m));
}

double CExtendedCenterline::GetCourse() const
{
	return course;
}

const std::vector<CenterlineElement>& CExtendedCenterline::GetElements() const
{
	return elements;
}

const std::string & CExtendedCenterline::GetFinalApproachFix() const
{
	return final_approach_fix;
}

const Identifier & CExtendedCenterline::GetIdentifier() const
{
	return id;
}

const std::vector<CenterlineMarker>& CExtendedCenterline::GetMarkers() const
{
	return markers;
}

double CExtendedCenterline::SetCourse(double crs)
{
	if (id == Identifier("*", "*"))
		return crs;
	return course = crs;
}

void CExtendedCenterline::SetFinalApproachFix(const std::string & fap)
{
	final_approach_fix = fap;
}
