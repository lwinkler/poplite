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
#include "generated/iface.server.hpp"


using namespace std;

client::client(pop::server _server) : points_(0), server_(_server)
{
}

client::~client()
{
	cout << "You have made " << points_ << " points" << endl;
}

void client::run()
{
	string word;
	while(true)
	{
		cin >> word;
		if(server_.guess(word))
		{
			cout << "correct" << endl;
			points_++;
		}
		else cout << "wrong" << endl;
	}
}
