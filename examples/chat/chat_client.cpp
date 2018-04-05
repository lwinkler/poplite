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
		contact.second->print(username_ + "<" + _text);
	}
}

void chat_client::print(const string& _text)
{
	cout << endl << _text << endl;
}

void chat_client::add_contact(const pop::accesspoint& _ap)
{
	// throw runtime_error("Test error");
	pop::write_lock lock1(contacts_lock_);
	shared_ptr<chat_client_iface> pcl(new chat_client_iface(_ap));
	const string username = pcl->get_username();
	for(auto& client : connected_clients_) {
		if(username == client.first) {
			LOG(warning) << "Contact " << username << " already connected";
			return;
		}
	}
	// note: maybe one day, improve this code and avoid using a map of pointers and use move operators
	connected_clients_.emplace(username, pcl);
}

void chat_client::remove_contacts()
{
	connected_clients_.clear();
}


map<string, pop::accesspoint> chat_client::get_contacts() {
	map<string, pop::accesspoint> res;
	for(auto& el : connected_clients_) {
		res.insert(make_pair(el.first, el.second->contact()));
	}
	return res;
}
