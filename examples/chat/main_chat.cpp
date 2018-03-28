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

	chat_client_iface c1;

	cout << "Contact this client at " << c1.contact().host_name << ":" << c1.contact().port << endl;

	cout << "'c':connect" << endl;
	cout << "'w':write" << endl;
	cout << "'q':quit" << endl;


	while(true) {

		try {
			string cmd ;
			cout << "cmd>";
			cin >> cmd;
			if(cmd == "q")
				break;
			else if(cmd == "c") {
				pop::accesspoint ap;
				cout << "host_name>";
				cin >> ap.host_name;
				cout << "port>";
				cin >> ap.port;
				c1.add_contact(ap);
			}
			else if(cmd == "w"){
				string msg;
				cout << ">>";
				cin >> msg;
				c1.send_all(msg);
			}
		} catch (exception e) {
			cerr << "Exception :" << e.what() << endl;
		}
	}



	return 0;
}
