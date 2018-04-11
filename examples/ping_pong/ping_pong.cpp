

#include "ping_pong.hpp"

void ping_pong::sync_ping(int _cpt)
{
	std::cout << "Counter = " << _cpt << std::endl;
	if(_cpt)
	{
		// Call next target
		ping_pong_iface iface(contact_);
		iface.sync_ping(_cpt - 1);
	}
}

void ping_pong::async_ping(int _cpt)
{
	std::cout << "Counter = " << _cpt << std::endl;
	if(_cpt)
	{
		// Call next target
		ping_pong_iface iface(contact_);
		iface.async_ping(_cpt - 1);
	}
}
