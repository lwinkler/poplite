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

#include <string>
#include <vector>
#include <map>
#include <set>

namespace pop
{
	class client;
}

/// A challenge to be completed by the user
struct challenge
{
	int num;
	char letter;
	std::string category;
};

/// The server that handles the game
class server
{
	public:
		//##POP_CONSTR(pop::local_allocator())
		server();

		//##POP_METH(sync, conc, bool, guess, std::string)
		bool guess(std::string _word);


	private:
		void send_message(const std::string& _msg);
		std::vector<pop::client*>                    p_clients_;
		std::map<std::string, std::set<std::string>> words_;
		std::map<std::string, std::vector<challenge>>     game_state_;

};

#endif
