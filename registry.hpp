#pragma once
#include <string>
#include <map>
#include <mutex>
class registry
{
public:

	bool register_service(const std::string& service_name, const std::string& host_name, int port)
	{
		std::unique_lock<std::mutex> lock;
		map_.emplace(service_name, entity{ host_name, port });
		return false;
	}

	bool unregister_service(const std::string& service_name, const std::string& host_name, int port)
	{
		std::unique_lock<std::mutex> lock;
		for (auto it = map_.cbegin(); it != map_.end();)
		{
			if (it->first == service_name && it->second == entity{ host_name ,port})
			{
				it = map_.erase(it);
			}
			else
			{
				++it;
			}
		}
		return false;
	}

private:
	struct entity
	{
		std::string host_name;
		int port;

		bool operator ==(const entity& e) const
		{
			return host_name == e.host_name&&port == e.port;
		}

		bool operator !=(const entity& e) const
		{
			return host_name != e.host_name||port == e.port;
		}
	};

	std::multimap<std::string, entity> map_;
	std::mutex mtx_;
};

