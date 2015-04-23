//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// #include "client.hpp"
// #include "class/system.hpp"
#include "class/interface.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		// Check command line arguments.
		if (argc != 3)
		{
			LOG(error) << "Usage: client <host> <port>";
			return 1;
		}

		// Create contact with broker
		boost::asio::ip::tcp::resolver::query query(argv[1], argv[2]);
		pop::interface iface(query);

		// Serialization of objects


		LOG(info) << "call GetValues";
		std::tuple<int,int,double,string> tup0;
		iface.call_sync<int,int,double,string>(6, tup0);
		boost::archive::text_oarchive aout(cout);
		aout << tup0;
		cout << endl;

		
		sleep(1);

		LOG(info) << "call SetValues method to set new values";
		tup0 = std::make_tuple(1, 42, 3.14, "new stuff");
		iface.call_sync<int,int,double,string>(5, tup0);
		aout << tup0;
		cout << endl;

		sleep(1);

		LOG(info) << "call GetValues again";
		std::tuple<int,int,double,string> tup1;
		iface.call_sync<int,int,double,string>(6, tup1);
		aout << tup1;
		cout << endl;

		sleep(1);

		LOG(info) << "call GetValues again";
		iface.call_sync<int,int,double,string>(6, tup1);
		aout << tup1;
		cout << endl;
		cout << "end of main" << endl;
	}
	catch (std::exception& e)
	{
		LOG(error) << e.what();
	}

	return 0;
}
