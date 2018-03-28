//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "test_class.hpp"

using namespace std;

bool test_interface(TestClass_iface& testClass, bool set_values)
{
	int i1 = 11, i2 = 22;
	double d = 88;
	string s = "bla";

	sleep(1);

	cout << "call SetValues method to set new values: i1=" << i1 << " i2=" << i2 << " d=" << d << " s=" << s << endl;
	if(set_values)
		testClass.SetValues(27, 42, 3.14, "new stuff");

	sleep(1);

	int val = testClass.GetValue();
	cout << "GetValue: " << val << endl;
	if(testClass.GetValue() != 333)
		return false;

	cout << "call GetValues" << endl;
	testClass.GetValues(i1, i2, d, s);
	cout << "i1=" << i1 << " i2=" << i2 << " d=" << d << " s=" << s << endl;
	if(!(i1 == 27 && i2 == 42 && d == 3.14 && s == "new stuff"))
		return false;

	cout << "s=" << testClass.GetStr() << endl;
	if(testClass.GetStr() != "new stuff")
		return false;

	cout << "Tests with a serializable class (gps position)" << endl;

	gps_position gps1(1, 1, 1);
	gps_position gps2(0, 0, 0);

	if(set_values)
		testClass.SetGps(gps1);
	gps2 = testClass.GetGps();
	if(!(gps1 == gps2))
		return false;

	sleep(1);

	cout << "Tests with a second serializable class" << endl;
	test_struct1 ts1, ts2;
	ts1.a = 444;
	if(set_values)
		testClass.SetTest(ts1);
	testClass.GetTest(ts2);
	if(ts2.a != 444)
		return false;

	return true;
}

/// A simple example for poplite

int main(int argc, char* argv[])
{
	try
	{
		cout << "call constructor" << endl;
		// iface.call_sync<int>(0, i1);
		TestClass_iface testClass("localhost");
		if(!test_interface(testClass, true))
			throw runtime_error("Test failed on testClass");

		if(!test_interface(testClass, false))
			throw runtime_error("Test failed on testClass(2)");

		cout << "create a second interface (reference)" << endl;
		TestClass_iface testClass2(testClass.contact());

		if(!test_interface(testClass2, false))
			throw runtime_error("Test failed on testClass2");

		cout << "create a third interface" << endl;
		TestClass_iface testClass3(testClass.contact());
		if(!test_interface(testClass3, false))
			throw runtime_error("Test failed on testClass3");

		cout << "end of main" << endl;
		sleep(3);
	}
	catch (std::exception& e)
	{
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}

	return 0;
}
