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

#include "alloc/local.hpp"
#include "com/accesspoint.hpp"

class client_iface;

/// A challenge to be completed by the user
struct challenge
{
	challenge(int _num, char _letter, std::string _category) : num(_num), letter(_letter), category(_category) {}
	void print(std::ostream & _os) const {_os << "Guess " << num << " word(s) of category " << category << " starting with " << letter << std::endl;}
	int num;
	char letter;
	std::string category;
};

// this include should always be right before the parallel class declaration
#include "server.iface.hpp"

/// The server that handles the game
POP_CLASS server
{
	public:
		POP_ALLOCATION(pop::local_allocator())
		// POP_ALLOCATION(pop::ssh_allocator("lwinkler@localhost"))
		server();
		virtual ~server();

		int guess(const std::string& _user, const std::string& _word);

		void connect_client(const std::string& _user, const pop::accesspoint& _client_contact);
		void init_game();
		void POP_ASYNC print_game(const std::string& _username = "");

	private:
		void POP_ASYNC send_message(const std::string& _msg, const std::string& _user = "");
		// void end_game();
		challenge create_challenge(int nb_, const std::string& category = "", char letter = ' ');

		std::map<std::string, client_iface*>           p_clients_;
		std::vector<std::string>                       categories_;
		std::string                                    category_;
		std::map<std::string, std::set<std::string>>   words_;
		std::map<std::string, std::vector<challenge>>  game_state_;

		unsigned int seed_;
};

#endif
