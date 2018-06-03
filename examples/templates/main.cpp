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
#include "printer.hpp"
#include "map_storage.hpp"
#include "proxy_storage.hpp"

using namespace std;

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
		{
			template_ns::proxy_storage_iface<int> stor1;
			stor1.push(11);
			stor1.push(22);
			stor1.push(33);
			stor1.print();
			cout << stor1.pop() << endl;
			stor1.print();
			cout << "--------------------" << endl;
		}

		{
			template_ns::proxy_storage_iface<std::string> stor2;
			stor2.push("eleven");
			stor2.push("twenty two");
			stor2.push("thirty three");
			stor2.print();
			cout << stor2.pop() << endl;
			stor2.print();
			cout << "--------------------" << endl;
		}

		{
			template_ns::map_storage_iface<int,string> stor1;
			stor1.insert(11, "eleven");
			stor1.insert(22, "twenty two");
			stor1.insert(33, "thirty three");
			stor1.print();
			cout << stor1.erase(22) << endl;
			stor1.print();
			cout << "--------------------" << endl;
		}

		{
			template_ns::map_storage_iface<string, float> stor1;
			stor1.insert("eleven", 11);
			stor1.insert("twenty two", 22.0);
			stor1.insert("thirty three", 33.0);
			stor1.print();
			cout << stor1.erase("twenty two") << endl;
			stor1.print();
			cout << "--------------------" << endl;
		}

		template_ns::printer_iface pr;
		pr.print(121212);
		pr.print(string("Tested the template method"));
	}
	catch (std::exception& e)
	{
		LOG(error) << "ERROR: " << e.what();
		return 1;
	}

	return 0;
}
