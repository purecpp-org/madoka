#pragma once
#include <string>
#include <tuple>
#include <atomic>
#include "registry.hpp"

class load_blancer
{
public:

	load_blancer(registry& reg) : resgistry_(reg), pos_(0)
	{
	}

	~load_blancer()
	{
	}

	std::tuple<std::string, int> fetch(const std::string& service_name)
	{
		auto map = resgistry_.get_services();
		if (pos_ >= map.size())
			pos_ = 0;

		if (map.empty())
			return{};

		auto range = map.equal_range(service_name);
		if (range.first == range.second)
			return{};

		auto it = range.first;
		
		std::advance(it, pos_.load());
		pos_++;
		return std::make_tuple(it->second.addr.host_name, it->second.addr.port);
	}

private:
	registry& resgistry_;
	std::atomic<size_t> pos_;
};

