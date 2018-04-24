//
// test_child_class.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TEST_CHILD_CLASS_H
#define TEST_CHILD_CLASS_H

#include "test_class.hpp"



// this include should always be right before the parallel class declaration
#include "TestChildClass.iface.hpp"

// TODO: Test with const method

POP_CLASS TestChildClass : public TestClass
{
	public:
		// POP_ALLOCATION(pop::ssh_allocator("lwinkler@localhost"))
		POP_ALLOCATION(pop::local_allocator())
		TestChildClass(std::string _s2, int myint) : TestClass("localhost") {std::cout << "call constr with " << _s2 << std::endl;}
		void Print() const {}

};

#endif
