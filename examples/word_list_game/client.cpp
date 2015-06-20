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
#include "generated/server.iface.hpp"


using namespace std;

client::client(string _user, pop::accesspoint _server_ap) : points_(0), server_(_server_ap), username_(_user)
{
	cout << "Enter your name: ";
	cin >> username_;
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
		if(server_.guess(username_, word))
		{
			cout << "correct" << endl;
			points_++;
		}
		else cout << "wrong" << endl;
	}
}
