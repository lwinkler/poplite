

#include "ping_pong.hpp"

void ping_pong::sync_ping(const pop::accesspoint& _target, int _cpt)
{
	std::cout << "Counter = " << _cpt << std::endl;
	if(_cpt)
	{
		// Call next target
		ping_pong_iface iface(_target);
		iface.sync_ping(contact_, _cpt - 1);
	}
}

void ping_pong::async_ping(const pop::accesspoint& _target, int _cpt)
{
	std::cout << "Counter = " << _cpt << std::endl;
	if(_cpt)
	{
		// Call next target
		ping_pong_iface iface(_target);
		iface.async_ping(contact_, _cpt - 1);
	}
}
