//
// client.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WORDLIST_CLIENT_HPP
#define WORDLIST_CLIENT_HPP

#include "server.hpp"

// this include should always be right before the parallel class declaration
#include "client.iface.hpp"

POP_CLASS client {
public:
	POP_ALLOCATION(pop::local_allocator())
	client(std::string _user, pop::accesspoint _server_ap);
	virtual ~client();

	POP_SYNC inline void message(std::string msg_) {
		std::cout << msg_ << std::endl;
	}

	POP_SYNC inline std::string get_username() {
		return username_;
	}

	POP_SYNC inline int get_points() {
		return points_;
	}

	POP_SYNC void run();

private:
	server_iface server_;
	int points_;
	const std::string username_;
};

#endif
