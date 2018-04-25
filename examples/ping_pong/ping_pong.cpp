

#include "ping_pong.hpp"

void ping_pong::sync_ping(int _cpt)
{
	std::cout << "Counter = " << _cpt << std::endl;
	if(_cpt)
	{
		// Call next target
		// trick: with sync we need to create a new connection to avoid deadlocks !
		// TODO: Maybe implement a multiplexer for sync communication
		ping_pong_iface pp(next_one_->contact());
		pp.sync_ping(_cpt - 1);
	}
}

void ping_pong::async_ping(int _cpt)
{
	std::cout << "Counter = " << _cpt << std::endl;
	if(_cpt)
	{
		// Call next target
		next_one_->async_ping(_cpt - 1);
	}
}
