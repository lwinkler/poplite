//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2018 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "tester_default.hpp"


using namespace std;

int main(int argc, char** argv) {

	try {
		// Init the pop system with arguments
		pop::system::instance(&argc, argv);

		tester_default_iface def;

		def.test_default_int();
		def.test_default_double_1();
		def.test_default_double_2();
		def.test_default_double_3();
		def.test_default_string();
		def.test_default_vector_int();
	} catch(const std::exception& exc) {
		cout << "Exception: " << exc.what() << endl;
		return 1;
	}

	return 0;
}
