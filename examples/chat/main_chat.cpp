//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "chat_client.hpp"

using namespace std;

/// Two remote objects are calling each other until counter reaches zero

int main(int argc, char* argv[])
{
	// Init the pop system with arguments
	pop::system::instance(argc, argv);

	string username = argc > 1 ? argv[1] : getenv("USER");
	chat_client_iface c1(username);

	cout << "Contact this client " << username << " at " << c1.contact().host_name << ":" << c1.contact().port << endl;
	cout << "'a':add contact" << endl;
	cout << "'l':list connected clients" << endl;
	cout << "'R':remove all contacts" << endl;
	cout << "'w':write" << endl;
	cout << "'q':quit" << endl;


	while(true) {

		try {
			cout << "cmd>";
			char c = getchar();
			if(c == 'q')
				break;
			else if(c == 'a') {
				pop::accesspoint ap;
				cout << "host_name>";
				cin >> ap.host_name;
				cout << "port>";
				cin >> ap.port;
				c1.add_contact(ap);

				// add to remote
				chat_client_iface remote(ap);
				remote.add_contact(c1.contact());
			} else if(c == 'R') {
				c1.remove_contacts();
			} else if(c == 'w') {
				string msg;
				cout << ">";
				while(msg.empty())
					getline(cin, msg);
				c1.send_all(msg);
			} else if(c == 'l') {
				map<string, pop::accesspoint> m = c1.get_contacts();
				cout << "Connected clients: " << endl;
				for(auto& el : m)
					cout << " - " << el.first << " on " << el.second.host_name << ":" << el.second.port << endl;
			}
		} catch (exception e) {
			cerr << "Exception in main: " << e.what() << endl;
		}
	}

	/* Alternative code
	try
	{
		chat_client_iface cl1;
		chat_client_iface cl2;
		cl1.add_contact(cl2.contact());
		cl2.add_contact(cl1.contact());
		cl1.send_all("This is a test message");
		cl2.send_all("This is another test message");
	}
	catch (std::exception& e)
	{
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}
	*/


	return 0;
}
