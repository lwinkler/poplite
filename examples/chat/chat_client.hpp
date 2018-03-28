//
// chat_client.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHAT_CLIENT_HPP
#define CHAT_CLIENT_HPP

#include <list>

#include "class/system.hpp"
#include "com/accesspoint.hpp"

// this include should always be right before the parallel class declaration
#include "chat_client.iface.hpp"

POP_CLASS chat_client
{
	public:
		POP_ALLOCATION(pop::local_allocator())
		chat_client() {}

		void POP_ASYNC send_all(const std::string& _text);
		void POP_ASYNC print(const std::string& _text);
		void POP_SYNC add_contact(const pop::accesspoint& _ap);
	
	private:
		std::list<chat_client_iface> connected_clients_;
};

#endif
