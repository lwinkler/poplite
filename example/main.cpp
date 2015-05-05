//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "test_class.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		int i1 = 11, i2 = 22;
		double d = 88;
		string s = "bla";

		LOG(info) << "call constructor";
		// iface.call_sync<int>(0, i1);
		TestClassInterface testClass(i1);

/*
		boost::asio::ip::tcp::endpoint& ep(testClass.contact());
		std::string inbound_header_;
		boost::asio::read(socket_, boost::asio::buffer(inbound_header_));
		cout << "asdfasdf " << inbound_header_ << endl;
		*/


		cout << "i1=" << i1 << " i2=" << i2 << " d=" << d << " s=" << s << endl;
		
		sleep(1);

		LOG(info) << "call SetValues method to set new values";
		// iface.call_sync<int,int,double,string>(1, 11, 42, 3.14, "new stuff");
		testClass.SetValues(2.7, 42, 3.14, "new stuff");

		sleep(1);

		LOG(info) << "call GetValues again";
		// iface.call_sync<int&,int&,double&,string&>(1, i1, i2, d, s);
		testClass.GetValues(i1, i2, d, s);
		cout << "i1=" << i1 << " i2=" << i2 << " d=" << d << " s=" << s << endl;

		sleep(1);

		// iface.call_sync<>(-1);
		cout << "end of main" << endl;
	}
	catch (std::exception& e)
	{
		LOG(error) << e.what();
	}

	return 0;
}
