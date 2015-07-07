//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "ping_pong.iface.hpp"

using namespace std;

/// Two remote objects are calling each other until counter reaches zero

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		cerr << "usage: " << argv[0] << " <nb pings>" << endl;
		return 1;
	}

	try
	{
		ping_pong_iface p1;
		p1.set_contact(p1.contact());
		ping_pong_iface p2;
		p2.set_contact(p2.contact());

		// p2.sync_ping(p1.contact(), atoi(argv[1]));
		p2.async_ping(p1.contact(), atoi(argv[1]));
		p2.async_ping(p1.contact(), atoi(argv[1]));
		// p2.sync_ping(p1.contact(), atoi(argv[1]));
		sleep(2); // TODO: Remove this and use a callback

	}
	catch (std::exception& e)
	{
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}

	return 0;
}
