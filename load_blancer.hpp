#pragma once
#include <string>
#include <tuple>

class load_blancer
{
public:

	load_blancer()
	{
	}

	~load_blancer()
	{
	}

	std::tuple<std::string, int> fetch(const std::string& service_name)
	{
		return std::make_tuple("host", 9000);
	}
};

