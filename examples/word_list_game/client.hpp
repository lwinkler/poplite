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

#include "com/accesspoint.hpp"
#include "server.iface.hpp"
#include "alloc/manual.hpp"


class pop_parallel client
{
	public:
		pop_allocation(pop::manual_allocator())
		client(std::string _user, pop::accesspoint _server_ap);
		~client();

		inline void message(std::string msg_)
		{
			std::cout << msg_ << std::endl;
		}

		inline std::string get_username() {return username_;}
		inline int get_points() {return points_;}

		void run();

	private:
		pop::server server_;
		int points_;
		std::string username_;
};

#endif
