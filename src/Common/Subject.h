#pragma once
#include "Observer.h"

#include <list>
#include <memory>
#include <string>

class Subject
{
public:
	void addObserver(Observer* observer)
	{
		observers.push_back(observer);
	}

	void removeObserver(Observer* observer)
	{
		observers.remove(observer);
	}

	void notify(const std::string_view& observerEvent)
	{
		for (const auto& observer : observers)
		{
			observer->onNotify(observerEvent);
		}
	}

private:
	std::list<Observer*> observers;
};