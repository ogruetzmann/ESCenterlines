#pragma once
class CCenterlineElement
{
public:
	double tick_length;
	double gap_length;
	double length;
	bool starts_with_gap;

	CCenterlineElement(double tick_length, double gap_length, double length, bool starts_with_gap);
	virtual ~CCenterlineElement();
};

