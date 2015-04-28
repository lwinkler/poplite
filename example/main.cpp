//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "class/interface.hpp"
#include "alloc/local.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		// Create contact with broker
		pop::local_allocator allocator;
		pop::interface iface(allocator);


		// Serialization of objects

		sleep(1);
		int i1 = 11, i2 = 22;
		double d = 88;
		string s = "bla";

		LOG(info) << "call constructor";
		iface.call_sync<int>(0, i1);
		cout << "i1=" << i1 << " i2=" << i2 << " d=" << d << " s=" << s << endl;
		
		sleep(1);

		LOG(info) << "call SetValues method to set new values";
		iface.call_sync<int,int,double,string>(6, 11, 42, 3.14, "new stuff");

		sleep(1);

		LOG(info) << "call GetValues again";
		iface.call_sync<int&,int&,double&,string&>(7, i1, i2, d, s);
		cout << "i1=" << i1 << " i2=" << i2 << " d=" << d << " s=" << s << endl;

		sleep(1);

		iface.call_sync<>(-1);
		cout << "end of main" << endl;
	}
	catch (std::exception& e)
	{
		LOG(error) << e.what();
	}

	return 0;
}
