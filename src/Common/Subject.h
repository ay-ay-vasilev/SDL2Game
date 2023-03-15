#pragma once
#include <boost/signals2.hpp>
#include <string>

class Subject
{
public:
	virtual ~Subject()
	{
		clearObservers();
	}

	void addObserver(boost::signals2::scoped_connection& connection, const std::function<void(const std::string_view&)>& slot)
	{
		connection = signal.connect(slot);
	}

	void removeObserver(boost::signals2::scoped_connection& connection)
	{
		connection.disconnect();
	}

	void notify(const std::string_view& observerEvent)
	{
		signal(observerEvent);
	}

private:
	void clearObservers()
	{
		signal.disconnect_all_slots();
	}

	friend class Observer;
	boost::signals2::signal<void(const std::string_view&)> signal;
};