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
#include "client.iface.hpp"

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
		cout << "Loading words for category " << categories_.back() << "(" << categories_.size() - 1 << ")" << endl;
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

	cout << "Pick a category (" << categories_.size() << " for mixed categories):" << endl;
	cin >> num;
	try
	{
		category_ = categories_.at(num);
	}
	catch(...)
	{
		cout << "Selected mixed categories" << endl;
		category_ = "";
	}
}

server::~server()
{
	// Free client references
	for(auto& elem : p_clients_)
		delete elem.second;
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

void server::connect_client(std::string _user, pop::accesspoint _ap)
{
	LOG(debug) << "server::connect";
	client_iface* pcl = nullptr; //  new pop::client(_ap);
	LOG(debug) << "created cl";
	p_clients_[_user] = pcl;
	send_message("Client " + _user + " connected\n");
}

void server::send_message(const string& _message)
{
	// cout << _message << endl;
	// return;
	for(auto elem : p_clients_)
		elem.second->message(_message);
}

challenge server::create_challenge(int nb_, std::string category, char letter)
{
	if(category.empty())
		category = categories_.at(rand_r(&seed_) % categories_.size());
	if(letter == ' ')
		letter = 'A' + rand_r(&seed_) % 26;
	return challenge(nb_, letter, category);
}

void server::init_game()
{
	game_state_.clear();
	// Fill game state arrays with challenges
	send_message("Starting game for " + to_string(p_clients_.size()) + " player(s)");
	for(int i = 0 ; i < 10 ; i++)
	{
		challenge chal = create_challenge(1, category_);
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
	stringstream ss;
	for(const auto& elem : game_state_[_username])
	{
		elem.print(ss);
	}
	send_message(ss.str());
}
