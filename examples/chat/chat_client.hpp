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

#include <map>

#include "class/system.hpp"
#include "com/accesspoint.hpp"

// this include should always be right before the parallel class declaration
#include "chat_client.iface.hpp"

POP_CLASS chat_client
{
	public:
		POP_ALLOCATION(pop::local_allocator())
		chat_client(const std::string& _username) : username_(username_) {}

		void POP_ASYNC send_all(const std::string& _text);
		void POP_ASYNC print(const std::string& _text);
		void POP_SYNC add_contact(const pop::accesspoint& _ap);
		void POP_SYNC remove_contacts();
		void POP_SYNC get_contacts(std::map<std::string, pop::accesspoint>& _contacts);
		std::string POP_SYNC get_username() {return username_;}
	
	private:
		std::map<std::string, std::shared_ptr<chat_client_iface>> connected_clients_;
		std::string username_;
		pop::lock contacts_lock_;
};

#endif
