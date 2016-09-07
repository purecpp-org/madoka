#pragma once
#include <string>
using namespace timax::rpc;

struct address
{
	std::string service_name;
	std::string host_name;
	int port;

	bool operator == (const address& adr) const
	{
		return adr.service_name == service_name&&adr.host_name == host_name&&adr.port == port;
	}

	MSGPACK_DEFINE(service_name, host_name, port);
};

struct entity
{
	address addr;
	std::shared_ptr<connection<msgpack_codec>> conn;

	bool operator == (const entity& en) const
	{
		return en.addr == addr;
	}
};
