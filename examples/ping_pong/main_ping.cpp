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
	if(argc != 3 || (string(argv[1]) != "async" && string(argv[1]) != "sync" ))
	{
		cerr << "usage: " << argv[0] << " <sync or async> <nb pings>" << endl;
		return 1;
	}

	try
	{
		ping_pong_iface p1;
		p1.set_contact(p1.contact());
		ping_pong_iface p2;
		p2.set_contact(p2.contact());

		if(string(argv[1]) == "async")
		{
			p2.async_ping(p1.contact(), atoi(argv[2]));
			sleep(2); // TODO: Remove this and use a callback
		}
		else
		{
			p2.sync_ping(p1.contact(), atoi(argv[2]));
		}
	}
	catch (std::exception& e)
	{
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}

	return 0;
}
