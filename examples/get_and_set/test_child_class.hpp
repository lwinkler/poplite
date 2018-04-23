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




// TODO: Test with const method

POP_CLASS TestChildClass : public TestClass
{
	public:
		struct wheel {
			float radius = 0;
			float price = 0;
			friend class boost::serialization::access;
			template<class Archive> void serialize(Archive & ar, const unsigned int version)
			{
				ar & radius;
				ar & price;
			}
		};


		#include "TestChildClass.iface.hpp"

		// POP_ALLOCATION(pop::ssh_allocator("lwinkler@localhost"))
		POP_ALLOCATION(pop::local_allocator())
		TestChildClass(std::string _s2, int myint) : TestClass("localhost") {std::cout << "call constr with " << _s2 << std::endl;}
		void Print(){}
		void SetWheel(const wheel& _wheel){wheel_ = _wheel;}

	private:
		wheel wheel_;
};

#endif
