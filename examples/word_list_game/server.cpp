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
#include "client.hpp"

using namespace std;


server::server() : seed_(time(NULL)) {
	vector<string> languages;
	int num = 1;
	cout << "Please choose language:" << endl;
	for(const auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator("data"), {})) {
		cout << num << ": " << entry.path().filename() << endl;
		languages.push_back(entry.path().filename().string());
		num++;
	}
	cin >> num;

	for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator("data/" + languages.at(num - 1)), {})) {
		categories_.push_back(entry.path().filename().string());
		cout << "Loading words for category " << categories_.back() << "(" << categories_.size() - 1 << ")" << endl;
		set<string>& list(words_[categories_.back()]);

		ifstream txtFile( entry.path().string());
		string tmpString;
		assert(txtFile.is_open());
		while(txtFile.good()) {
			// Read line from file
			getline(txtFile, tmpString);
			list.insert(tmpString);
		}
		txtFile.close();

		cout << "Found " << list.size() << " words" << endl;
	}

	cout << "Pick a category (" << categories_.size() << " for mixed categories):" << endl;
	cin >> num;
	try {
		category_ = categories_.at(num);
	} catch(...) {
		cout << "Selected mixed categories" << endl;
		category_ = "";
	}
}

server::~server() {
	// Free client references
	for(auto& elem : p_clients_)
		delete elem.second;
}

int server::guess(const string& _user, const string& _word) {
	bool complete = true;
	int points = 0;
	string word = boost::to_upper_copy<std::string>(_word);
	for(auto& chal : game_state_[_user]) {
		if(chal.num > 0) {
			auto& dict(words_[chal.category]);
			// cout << (_word.at(0) == chal.letter) << (find(dict.begin(), dict.end(), _word) != dict.end())<<endl;
			if(word.at(0) == chal.letter && find(dict.begin(), dict.end(), word) != dict.end()) {
				p_clients_[_user]->message(_word + " is a correct answer for " + chal.category);
				chal.num--;
				points++;
			} else complete = false;
		}
	}
	if(complete) {
		p_clients_[_user]->message("Congratulation, you won !");
		send_message("The game has been won by player " + _user);
		return points;
	}

	// Re-print the state of the game
	print_game(_user);

	return points;
}

void server::connect_client(const std::string& _user, const pop::accesspoint& _ap) {
	LOG(debug) << "server::connect";
	client_iface* pcl = new client_iface(_ap);
	LOG(debug) << "created cl";
	p_clients_[_user] = pcl;
	cout << "Client " << p_clients_.size() << " " << _user << " connected" << endl;
}

void server::send_message(const string& _message, const string& _user) {
	if(_user.empty())
		for(auto elem : p_clients_)
			elem.second->message(_message);
	else
		p_clients_[_user]->message(_message);
}

challenge server::create_challenge(int nb_, const std::string& category, char letter) {
	string cat = category;
	if(cat.empty())
		cat = categories_.at(rand_r(&seed_) % categories_.size());
	if(letter == ' ')
		letter = 'A' + rand_r(&seed_) % 26;
	return challenge(nb_, letter, cat);
}

void server::init_game() {
	game_state_.clear();
	// Fill game state arrays with challenges
	send_message("Starting game for " + to_string(p_clients_.size()) + " player(s)");
	for(int i = 0 ; i < nb_questions_ ; i++) {
		challenge chal = create_challenge(1, category_);

		for(auto &elem1 : p_clients_) {
			game_state_[elem1.first].push_back(chal);
		}
	}
	print_game();
}

void server::print_game(const string& _username) {
	stringstream ss;
	for(const auto& elem : game_state_[_username]) {
		elem.print(ss);
	}
	send_message(ss.str(), _username);
}
