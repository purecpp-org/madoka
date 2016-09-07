#pragma once
#include <string>
#include <map>
#include <mutex>

#include "common.h"

class registry
{
public:
	registry() = default;
	address register_service(const std::string& service_name, const std::string& host_name, int port)
	{
		return { service_name, host_name, port };
	}

	bool unregister_service(const std::string& service_name, const std::string& host_name, int port)
	{
		std::unique_lock<std::mutex> lock(mtx_);
		for (auto it = map_.cbegin(); it != map_.end();)
		{
			if (it->first == service_name && it->second == entity{ service_name, host_name ,port})
			{
				it = map_.erase(it);
			}
			else
			{
				++it;
			}
		}
		return true;
	}

	void set_conn(std::shared_ptr<connection<msgpack_codec>> conn, const address& addr)
	{
		entity en = { addr, conn };
		std::unique_lock<std::mutex> lock(mtx_);
		map_.emplace(addr.service_name, en);
	}

	void handle_disconnect(connection<msgpack_codec>* conn)
	{
		std::unique_lock<std::mutex> lock(mtx_);
		for (auto it = map_.cbegin(); it != map_.end();)
		{
			if (it->second.conn.get() == conn)
			{
				std::cout << it->second.addr.host_name << " offline" << std::endl;
				it = map_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

private:
	friend class load_blancer;

	std::multimap<std::string, entity> get_services()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		return map_;
	}
	registry(const registry&) = delete;
	std::multimap<std::string, entity> map_;
	std::mutex mtx_;
};

