//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include "server.hpp"
#include "generated/client.iface.hpp"

using namespace std;


server::server() : seed_(time(NULL))
{
	vector<string> languages;
	int num = 1;
	cout << "Please choose language:" << endl;
	for(const auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator("data"), {}))
	{
		std::cout << num << ": " << entry.path().filename() << endl;
		languages.push_back(entry.path().filename().string());
		num++;
	}
	cin >> num;

	for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator("data/" + languages.at(num - 1)), {}))
	{
		categories_.push_back(entry.path().filename().string());
		cout << "Loading words for category " << categories_.back() << endl;
		set<string>& list(words_[categories_.back()]);

		ifstream txtFile( entry.path().string());
		string tmpString;
		assert(txtFile.is_open());
		while(txtFile.good())
		{
			// Read line from file
			getline(txtFile, tmpString);
			list.insert(tmpString);
		}
		txtFile.close();

		cout << "Found " << list.size() << " words" << endl;
	}

	// pop::accesspoint ap;
	p_clients_["a"] = nullptr; // TODO remove this hack
}

server::~server()
{
	// Free client references
	for(auto& elem : p_clients_)
		; // TODO delete elem.second;
}

int server::guess(string _user, string _word)
{
	bool complete = true;
	int points = 0;
	string word = boost::to_upper_copy<std::string>(_word);
	for(auto& chal : game_state_[_user])
	{
		if(chal.num > 0)
		{
			auto& dict(words_[chal.category]);
			// cout << (_word.at(0) == chal.letter) << (find(dict.begin(), dict.end(), _word) != dict.end())<<endl;
			if(word.at(0) == chal.letter && find(dict.begin(), dict.end(), word) != dict.end())
			{
				send_message(_word + " is a correct answer for " + chal.category);
				chal.num--;
				points++;
			}
			else complete = false;
		}
	}
	if(complete)
		send_message("Congratulation, you won !");
	print_game(_user);
	return points;
}

void server::connect(std::string _user /*, pop::accesspoint _ap*/)
{
	// pop::accesspoint _ap; // TODO
	LOG(debug) << "server::connect";
	// pop::client* pcl = new pop::client(_ap);
	// p_clients_[_user] = pcl;
}

void server::send_message(const string& _message)
{
	cout << _message << endl;
	return; //TODO
	for(auto elem : p_clients_)
		elem.second->message(_message);
}

challenge server::create_challenge(int nb_)
{
	return challenge(nb_, 'A' + rand_r(&seed_) % 26, categories_.at(rand_r(&seed_) % categories_.size()));
}

void server::init_game()
{
	game_state_.clear();
	// Fill game state arrays with challenges
	for(int i = 0 ; i < 10 ; i++)
	{
		challenge chal = create_challenge(1);
		stringstream ss;
		send_message(ss.str());

		for(auto &elem1 : p_clients_)
		{
			game_state_[elem1.first].push_back(chal);
		}
	}
}

void server::print_game(string _username)
{
	for(const auto& elem : game_state_[_username])
	{
		elem.print(cout);
	}
}


/*
TODO

add a structure

map<user_string, struct>
struct: 
	vector<challenge>
	int points

challenge: int nb, char letter, string categ


*/
