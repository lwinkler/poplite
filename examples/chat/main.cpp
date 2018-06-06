//
// main_chat.cpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "chat_client.hpp"

using namespace std;

/// Two remote objects are calling each other until counter reaches zero

int main(int argc, char* argv[]) {
	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	string username = argc > 1 ? argv[1] : getenv("USER");
	chat_client_iface c1(username);

	cout << "Contact this client " << username << " at " << c1.contact().host_name << ":" << c1.contact().port << endl;
	cout << "'a':add contact" << endl;
	cout << "'l':list connected clients" << endl;
	cout << "'R':remove all contacts" << endl;
	cout << "'D':disconnect" << endl;
	cout << "'w':write" << endl;
	cout << "'q':quit" << endl;

	char c = 0;
	while(c != 'q') {

		try {
			c = getchar();
			switch(c) {
				case 'D':
				case 'q': {
					// c1.disconnect(); // note: cannot be added to destructor: deadlock
					break;
				}
				case 'a': {
					pop::accesspoint ap;
					cout << "host_name>";
					cin >> ap.host_name;
					cout << "port>";
					cin >> ap.port;
					c1.add_contact(ap);

					// add to remote
					chat_client_iface remote(ap);
					remote.add_contact(c1.contact());
					break;
				}
				case 'R': {
					c1.remove_contacts();
					break;
				}
				case 'w': {
					string msg;
					cout << ">";
					while(msg.empty())
						getline(cin, msg);
					c1.send_all(msg);
					break;
				}
				case 'l': {
					map<string, pop::accesspoint> m = c1.get_contacts();
					cout << "Connected clients: " << (m.empty() ? "none" : "") << endl;
					for(auto& el : m)
						cout << " - " << el.first << " on " << el.second.host_name << ":" << el.second.port << endl;
					break;
				}
			};
		} catch (const exception& exc) {
			cerr << "Exception in main: " << exc.what() << endl;
			return 1;
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
