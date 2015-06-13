//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WORDLIST_SERVER_HPP
#define WORDLIST_SERVER_HPP

#include "class/broker.hpp"
#include "class/interface.hpp"
#include "alloc/local.hpp"

namespace pop
{
	class client;
}

class server
{
	public:
		//##POP_CONSTR(pop::local_allocator(), std::string)
		server() {}
		//##POP_METH(sync, conc, bool, guess, std::string)
		bool guess(std::string _word);


		//  ##POP_METH(sync, conc, std::string, GetStr)
		// std::string GetStr() {return s_;}

	private:
		void send_message(const std::string& _msg);
		std::vector<pop::client*> p_clients_;
};

#endif
