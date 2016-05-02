#include "CenterlineElement.h"



CCenterlineElement::CCenterlineElement(double tick_length, double gap_length, double length, bool starts_with_gap)
	: tick_length(tick_length), gap_length(gap_length), length(length), starts_with_gap(starts_with_gap)
{
}


CCenterlineElement::~CCenterlineElement()
{
}
