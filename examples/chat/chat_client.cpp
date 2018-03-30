//
// chat_client.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

using namespace std;

#include "chat_client.hpp"

void chat_client::send_all(const string& _text)
{
	LOG(debug) << "send to " << connected_clients_.size() << " clients";
	for(auto& contact : connected_clients_)
	{
		contact.print(_text);
	}
}

void chat_client::print(const string& _text)
{
	cout << ">> " << _text << endl;
}

void chat_client::add_contact(const pop::accesspoint& _ap)
{
	try {
		connected_clients_.emplace_back(_ap);
	} catch(exception& e) {
		cerr << "Exception while adding contact " << e.what() << endl;
	}
}

void chat_client::remove_contacts()
{
	connected_clients_.clear();
}
