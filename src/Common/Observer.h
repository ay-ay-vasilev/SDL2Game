#pragma once
#include <boost/signals2.hpp>
#include <string>
#include <vector>
#include <functional>
#include <boost/bind/placeholders.hpp>
#include <boost/mp11/bind.hpp>

class Observer
{
public:
	virtual ~Observer()
	{
		for (auto& connection : connections)
		{
			connection.disconnect();
		}
	}

	void registerWithSubject(const std::shared_ptr<Subject>& subject)
	{
		auto connection = subject->signal.connect(boost::bind(&Observer::onNotify, this, boost::placeholders::_1));
		connections.push_back(boost::signals2::scoped_connection(connection));
	}


	virtual void onNotify(const std::string_view& observedEvent) = 0;

private:
	std::vector<boost::signals2::scoped_connection> connections;
};