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
	if(argc == 1)
	{
		// Create a server
		pop::server serv;
		cout << "Users can contact the server by typing:" << endl;
		cout << argv[0] << " " << serv.contact() << endl;

		cout << "Create a first client" << endl;
		client cl(serv);

		cl.run();
	}
	else if(argc >= 3)
	{
		cout << "Create a client and connect to an existing server" << endl;
		accesspoint ap;
		ap.host_name_ = argv[1];
		ap.port       = atoi(argv[2]);

		server serv(ap);
		client cl(serv);

		cl.run();
	}

	return 0;
}
