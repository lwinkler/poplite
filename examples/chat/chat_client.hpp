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

#include <iostream>
#include <map>
#include <string>

#ifndef _POP_PARSER_ // this accelerates parsing
#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"
#endif

#include "parser/defs.hpp"
#include "com/accesspoint.hpp"

// this include should always be right before the parallel class declaration
#include "chat_client.iface.hpp"

POP_CLASS chat_client
{
public:
	POP_ALLOCATION(pop::local_allocator())
	chat_client(const std::string& _username) : username_(_username) {}

	void POP_ASYNC send_all(const std::string& _text);
	void POP_ASYNC print(const std::string& _text);
	void POP_SYNC add_contact(const pop::accesspoint& _ap);
	void POP_ASYNC remove_contacts();
	void POP_ASYNC remove_contact(const std::string& _username);
	void POP_ASYNC disconnect();
	std::map<std::string, pop::accesspoint> POP_SYNC get_contacts();
	std::string POP_SYNC get_username() {
		return username_;
	}

private:
	std::map<std::string, std::shared_ptr<chat_client_iface>> connected_clients_;
	std::string username_;
	pop::lock contacts_lock_;
};

#endif
