//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include "com/broker_combox.hpp"
#include "example/test_class.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		// Check command line arguments.
		if (argc != 3)
		{

			LOG(error) << argc << "Usage: " << argv[0] << " <hostname of interface> <port of interface>";
			return -1;
		}
		pop::remote::broker<TestClass> brok(TestClass::parallel_methods());
		boost::asio::ip::tcp::resolver::query query(argv[1], argv[2]);
		pop::broker_combox<TestClass> combox(brok, query);
		combox.run();
	}
	catch (std::exception& e)
	{
		LOG(error) << e.what();
	}

	return 0;
}
