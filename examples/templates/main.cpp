//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "storage.hpp"

using namespace std;

POP_SPECIFICATIONS_storage

/*
template<typename T> test_interface(storage_iface<T>& testClass, bool set_values)
{
}
*/

/// A simple example for poplite

int main(int argc, char* argv[])
{
	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	try
	{
		template_ns::storage_iface<int> stor1;
		stor1.push(11);
		stor1.push(22);
		stor1.push(33);
		stor1.print();

		LOG(info) << "end of main";
	}
	catch (std::exception& e)
	{
		LOG(error) << "ERROR: " << e.what();
		return 1;
	}

	return 0;
}
