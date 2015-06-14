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

#include "server.hpp"
#include "generated/iface.client.hpp"

using namespace std;


server::server()
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
		string categ = entry.path().filename().string();
		cout << "Loading words for category " << categ << endl;
		set<string>& list(words_[categ]);

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
}

bool server::guess(string _word){return true;}

void server::send_message(const string& _message)
{
	for(auto elem : p_clients_)
		elem->message(_message);
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
