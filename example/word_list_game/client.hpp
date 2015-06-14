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

#include "alloc/local.hpp"
#include "com/accesspoint.hpp"
#include "generated/iface.server.hpp"


class client
{
	public:
		//##POP_CONSTR(pop::local_allocator(), std::string, pop::accesspoint)
		client(std::string _user, pop::accesspoint _server_ap);
		~client();

		//##POP_METH(sync, conc, void, message, std::string)
		inline void message(std::string msg_)
		{
			std::cout << msg_ << std::endl;
		}

		//##POP_METH(sync, conc, int, get_points)
		inline int get_points() {return points_;}

		//##POP_METH(sync, conc, void, run)
		void run();

	private:
		pop::server server_;
		int points_;
		std::string user_;
};

#endif
