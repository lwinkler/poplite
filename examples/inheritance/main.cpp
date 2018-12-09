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
using namespace global_ns;

template<typename T> void check_equal(const T& _a, const T& _b, int _line) {
	if(_a != _b) {
		LOG(error) << _a << " is not equal to " << _b << ", line " << _line;
		throw runtime_error("Test failed");
	}
}


/// A simple example for poplite

int main(int argc, char* argv[]) {
	try {
		// Init the pop system with arguments
		pop::system::instance(&argc, argv);

		child_ns::child real1("my_object");
		child_ns::child_iface iface1("my_object");

		real1.child_method();
		iface1.child_method();

		real1.set_int(333);
		iface1.set_int(333);
		check_equal(real1.get_int(), iface1.get_int(), __LINE__);

		real1.set_double(3.14);
		iface1.set_double(3.14);
		check_equal(real1.get_double(), iface1.get_double(), __LINE__);

		real1.set_float(3e33);
		iface1.set_float(3e33);
		check_equal(real1.get_float(), iface1.get_float(), __LINE__);

		real1.set_float_a(3e3);
		iface1.set_float_a(3e3);
		check_equal(real1.get_float_a(), iface1.get_float_a(), __LINE__);

		check_equal(real1.get_non_virtual_name(), iface1.get_non_virtual_name(), __LINE__);
		check_equal(real1.get_virtual_name(), iface1.get_virtual_name(), __LINE__);
		check_equal(real1.get_static_name(), iface1.get_static_name(), __LINE__);
		check_equal(real1.more_magic(), iface1.more_magic(), __LINE__);
		check_equal(real1.pure_virtual(), iface1.pure_virtual(), __LINE__);

		parent_a&       real2(dynamic_cast<parent_a&>(real1));
		parent_a_iface& iface2(dynamic_cast<parent_a_iface&>(iface1));

		check_equal(real2.get_non_virtual_name(), iface2.get_non_virtual_name(), __LINE__);
		check_equal(real2.get_virtual_name(), iface2.get_virtual_name(), __LINE__);
		check_equal(real2.get_static_name(), iface2.get_static_name(), __LINE__);
		check_equal(real2.more_magic(), iface2.more_magic(), __LINE__);
		check_equal(real2.pure_virtual(), iface2.pure_virtual(), __LINE__);

		parent_b_ns::parent_b&       real3(dynamic_cast<parent_b_ns::parent_b&>(real2));
		parent_b_ns::parent_b_iface& iface3(dynamic_cast<parent_b_ns::parent_b_iface&>(iface2));

		check_equal(real3.get_non_virtual_name(), iface3.get_non_virtual_name(), __LINE__);
		check_equal(real3.get_virtual_name(), iface3.get_virtual_name(), __LINE__);
		check_equal(real3.get_static_name(), iface3.get_static_name(), __LINE__);
		check_equal(real3.more_magic(), iface3.more_magic(), __LINE__);
		check_equal(real3.pure_virtual(), iface3.pure_virtual(), __LINE__);
	} catch(exception& exc) {
		LOG(error) << "Exception in main: " << exc.what();
		return 1;
	}

	return 0;
}
