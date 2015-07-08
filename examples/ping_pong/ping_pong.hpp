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
#include "com/accesspoint.hpp"

POP_CLASS ping_pong
{
	public:
		POP_ALLOCATION(pop::local_allocator())
		ping_pong() {}

		void POP_SYNC  sync_ping(const pop::accesspoint& _target, int _cpt);
		void POP_ASYNC async_ping(const pop::accesspoint& _target, int _cpt);
		void set_contact(const pop::accesspoint& _ap){contact_ = _ap;}
	
	private:
		pop::accesspoint contact_;// TODO remove this
};

#endif
