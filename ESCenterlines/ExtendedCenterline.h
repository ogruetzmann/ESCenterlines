#pragma once
#include <vector>
#include <string>
#include "Identifier.h"
#include "DataObjects.h"

class CExtendedCenterline
{
public:
	CExtendedCenterline(Identifier& id);
	virtual ~CExtendedCenterline();

	void AddElement(CenterlineElement&& element);
	void AddMarker(CenterlineMarker&& marker);
	double SetCourse(double course);
	void SetFinalApproachFix(const std::string& fap);

	double GetCourse() const;
	const std::vector<CenterlineElement>& GetElements() const;
	const std::string& GetFinalApproachFix() const;
	const Identifier& GetIdentifier() const;
	const std::vector<CenterlineMarker>& GetMarkers() const;
	
private:
	Identifier id;
	std::vector<CenterlineMarker> markers;
	std::vector<CenterlineElement> elements;
	std::string final_approach_fix {};
	double course { 0.0 };
};

