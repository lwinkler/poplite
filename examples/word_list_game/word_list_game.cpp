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

#include "server.hpp"
#include "client.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	pop::system::instance(&argc, argv);

	try
	{
		if(argc == 1)
		{
			// Create a server
			server_iface serv;
			cout << "waiting for other players: game will start as soon as you log in" << endl;
			cout << "Users can contact the server by typing:" << endl;
			cout << argv[0] << " " << serv.contact().host_name << " " << serv.contact().port << endl;

			string user;
			cout << "Enter your name: ";
			cin >> user;
			cout << "Create a client" << endl;
			client_iface cl(user, serv.contact());
			serv.connect_client(user, cl.contact());

			serv.init_game();
			serv.print_game(user);
			cout << "Run the client" << endl;
			cl.run();
		}
		else if(argc >= 3)
		{
			cout << "Create a client and connect to an existing server " << argv[1] << ":" << argv[2] << endl;
			pop::accesspoint ap;
			ap.host_name = argv[1];
			ap.port      = atoi(argv[2]);
			server_iface serv(ap);

			string user;
			cout << "Enter your name: ";
			cin >> user;
			cout << "Create a client" << endl;
			client_iface cl(user, serv.contact());
			serv.connect_client(user, cl.contact());
			serv.print_game(user);
			cout << "Run the client" << endl;
			cl.run();
		} else {
			LOG(error) << "Unexpected number of arguments " << argc;
		}

		return 0;
	}
	catch(exception &e)
	{
		cerr << "Exception: " << e.what() << endl;
	}
}
