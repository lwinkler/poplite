//
// ping_pong.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "ping_pong.hpp"
#include "mutex.hpp"

using namespace std;

void ping_pong::sync_ping(int _cpt) {
	cout << "Counter = " << _cpt << endl;
	if(_cpt) {
		// Call next target
		// trick: with sync we need to create a new connection to avoid deadlocks !
		// TODO: Maybe implement a multiplexer for sync communication
		ping_pong_iface pp(next_one_->contact());
		pp.sync_ping(_cpt - 1);
	}
}

void ping_pong::async_ping(int _cpt, pop::accesspoint _callback) {
	cout << "Counter = " << _cpt << endl;
	if(_cpt) {
		// note: we need to add a delay, probably to let the connections close
		usleep(100);
		// Call next target
		next_one_->async_ping(_cpt - 1, _callback);
	} else {
		pop::mutex_iface l(_callback);
		l.unlock();
	}
}
