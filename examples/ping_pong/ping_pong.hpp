//
// ping_pong.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PING_PONG_HPP
#define PING_PONG_HPP

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "com/accesspoint.hpp"

// this include should always be right before the parallel class declaration
#include "ping_pong.iface.hpp"

/// A simple class that calls another object
POP_CLASS ping_pong
{
	public:
		// Constructor: We need to specify an allocator to specify how to launch the object
		// here we use a local allocator: the object will be created locally
		POP_ALLOCATION(pop::local_allocator())
		ping_pong() {}

		// An example of synchronous method
		void POP_SYNC  sync_ping(int _cpt);
		void POP_ASYNC async_ping(int _cpt);
		void set_next_one(const pop::accesspoint& _ap){next_one_.reset(new ping_pong_iface(_ap));}

	private:
		// The point of contact of the object
		std::unique_ptr<ping_pong_iface> next_one_;
};

#endif
