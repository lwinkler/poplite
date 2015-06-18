//
// word_list_game.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/// A game where the user has to guess as many words in one theme

#include "iface.server.hpp"
#include "iface.client.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		if(argc == 1)
		{
			// Create a server
			pop::server serv;
			cout << "Users can contact the server by typing:" << endl;
			cout << argv[0] << " " << serv.contact().host_name_ << " " << serv.contact().port_ << endl;


			cout << "Create a first client" << endl;
			pop::client cl(serv.contact());
			sleep(4);

			return 0;

			cl.run();
		}
		else if(argc >= 3)
		{
			cout << "Create a client and connect to an existing server" << endl;
			pop::accesspoint ap;
			ap.host_name_ = argv[1];
			ap.port_      = atoi(argv[2]);

			pop::server serv(ap);
			pop::client cl(serv.contact());

			cl.run();
		}

		return 0;
	}
	catch(exception &e)
	{
		cerr << "Exception: " << e.what() << endl;
	}
}