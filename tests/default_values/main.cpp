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

	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	tester_default_iface def;

	// TODO: There is probably a lot to improve with the parsing of default values
	def.test_default_int();
	def.test_default_double();
	def.test_default_string();
	// def.test_default_vector_int();

	return 0;
}
