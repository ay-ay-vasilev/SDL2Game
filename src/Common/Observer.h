#pragma once
#include <string>

class Observer
{
public:
	virtual void onNotify(const std::string_view& observedEvent) = 0;
};