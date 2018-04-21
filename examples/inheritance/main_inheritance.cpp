//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "child.hpp"

using namespace std;

template<typename T> void check_equal(const T& _a, const T& _b) {
	if(_a != _b) {
		LOG(error) << _a << " is not equal to " << _b;
		throw runtime_error("Test failed");
	}
}


/// A simple example for poplite

int main(int argc, char* argv[])
{
	try {
		// Init the pop system with arguments
		pop::system::instance(&argc, argv);

		child child1("abcd");
		child_iface iface1("abcd");

		child1.child_method();
		iface1.child_method();

		child1.set_int(333);
		iface1.set_int(333);
		check_equal(child1.get_int(), iface1.get_int());

		child1.set_double(3.14);
		iface1.set_double(3.14);
		check_equal(child1.get_double(), iface1.get_double());

		check_equal(child1.get_non_virtual_name(), iface1.get_non_virtual_name());
		check_equal(child1.get_virtual_name(), iface1.get_virtual_name());

		parent_a&       child2(dynamic_cast<parent_a&>(child1));
		parent_a_iface& iface2(dynamic_cast<parent_a_iface&>(iface1));

		check_equal(child2.get_non_virtual_name(), iface2.get_non_virtual_name());
		check_equal(child2.get_virtual_name(), iface2.get_virtual_name());
	} catch(exception& exc) {
		LOG(error) << "Exception in main: " << exc.what(); 
		return 1;
	}

	return 0;
}
