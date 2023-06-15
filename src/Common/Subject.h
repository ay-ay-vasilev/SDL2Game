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

	void inline addObserver(boost::signals2::scoped_connection& connection, const std::function<void(const std::string_view&)>& slot)
	{
		connection = signal.connect(slot);
	}

	void inline removeObserver(boost::signals2::scoped_connection& connection)
	{
		connection.disconnect();
	}

	void inline notify(const std::string_view& observerEvent)
	{
		signal(observerEvent);
	}

private:
	void inline clearObservers()
	{
		signal.disconnect_all_slots();
	}

	friend class Observer;
	boost::signals2::signal<void(const std::string_view&)> signal;
};