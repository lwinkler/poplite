//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include "client.hpp"
#include "server.hpp"


using namespace std;

client::client(string _user, pop::accesspoint _server_ap) :
	points_(0),
	server_(_server_ap),
	username_(_user)
{
}

client::~client()
{
	cout << "You have made " << points_ << " points" << endl;
}

void client::run()
{
	LOG(debug) << "run";
	string word;
	while(true)
	{
		LOG(debug) << "guess";
		cout << "word: ";
		cin >> word;
		points_ += server_.guess(username_, word);
	}
}
