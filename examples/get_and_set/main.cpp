//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "test_class.iface.hpp"

using namespace std;

/// A simple example for poplite

int main(int argc, char* argv[])
{
	try
	{
		int i1 = 11, i2 = 22;
		double d = 88;
		string s = "bla";

		LOG(info) << "call constructor";
		// iface.call_sync<int>(0, i1);
		TestClass_iface testClass("localhost");

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

		// cout << "s=" << testClass.GetStr() << endl; // TODO: Fix return value
		// TODO: forbid copies of interfaces

		gps_position gps1(1, 1, 1);
		gps_position gps2(0, 0, 0);

		testClass.SetGps(gps1);
		gps2 = testClass.GetGps();
		assert(gps1 == gps2);

		sleep(1);

		LOG(info) << "create a second interface";
		pop::interface testClass2(testClass.contact());
		// testClass2.sync<void , int&, int&, double&, std::string&>(pop::broker::GetValues2 , i1, i2, d, s);
		cout << "i1=" << i1 << " i2=" << i2 << " d=" << d << " s=" << s << endl;

		sleep(1);

		LOG(info) << "create a third interface";
		pop::interface testClass3(testClass.contact());
		// pop::interface testClass2(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 20015));
		// testClass3.sync<void , int&, int&, double&, std::string&>(pop::broker::GetValues2 , i1, i2, d, s);
		cout << "i1=" << i1 << " i2=" << i2 << " d=" << d << " s=" << s << endl;

		// iface.call_sync<>(-1);
		cout << "end of main" << endl;
		sleep(3);
	}
	catch (std::exception& e)
	{
		LOG(error) << e.what();
	}

	return 0;
}
