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
	pop::write_lock lock1(contacts_lock_);
	try {
		shared_ptr<chat_client_iface> pcl(new chat_client_iface(_ap));
		const string username = pcl->get_username();
		for(auto& client : connected_clients_) {
			if(username == client.first) {
				LOG(warning) << "Contact " << username << " already connected";
				return;
			}
		}
		// note: maybe one day, improve this code and avoid using a map of pointers
		connected_clients_.emplace(username, pcl);
	} catch(exception& e) {
		cerr << "Exception while adding contact " << e.what() << endl;
	}
}

void chat_client::remove_contacts()
{
	connected_clients_.clear();
}


void chat_client::get_contacts(map<string, accesspoint>& _contacts) {
	map<string, accesspoint> res;
	for(auto& el : connected_clients_) {
		res.insert(make_pair(el.first, el->second.get_contact()));
	}
}
