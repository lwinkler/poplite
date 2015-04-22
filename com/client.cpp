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


int main(int argc, char* argv[])
{
	try
	{
		// Check command line arguments.
		if (argc != 3)
		{
			std::cerr << "Usage: client <host> <port>" << std::endl;
			return 1;
		}

		// boost::asio::io_service io_service;
		// pop::client client(io_service, argv[1], argv[2]);
		// io_service.run();
				std::cout<<"handleasdfasdf connect"<<std::endl;
		boost::asio::ip::tcp::resolver::query query(argv[1], argv[2]);
				std::cout<<__LINE__<<std::endl;
		pop::interface iface(query);
				std::cout<<__LINE__<<std::endl;
		std::tuple<int,int,double,string> tup0;
				std::cout<<__LINE__<<std::endl;
		iface.call_sync<int,int,double,string>(0, tup0);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
