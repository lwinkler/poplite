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
#include "com/accesspoint.hpp"

namespace pop
{
	class client;
}

/// A challenge to be completed by the user
struct challenge
{
	challenge(int _num, char _letter, std::string _category) : num(_num), letter(_letter), category(_category) {}
	void print(){std::cout << category << " starting with " << letter << std::endl;}
	int num;
	char letter;
	std::string category;
};

/// The server that handles the game
class pop_parallel server
{
	public:
		pop_allocation(pop::local_allocator())
		server();
		~server();

		bool guess(std::string _user, std::string _word);

		void connect(std::string _user, pop::accesspoint _ap);

	private:
		void send_message(const std::string& _msg);
		void start_game();
		void end_game();
		challenge create_challenge(int nb_);

		std::map<std::string,pop::client*>             p_clients_;
		std::vector<std::string>                       categories_;
		std::map<std::string, std::set<std::string>>   words_;
		std::map<std::string, std::vector<challenge>>  game_state_;

		unsigned int seed_;
};

#endif
