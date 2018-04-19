//
// child.hpp
// ~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TEST_CHILD_H
#define TEST_CHILD_H

#include "class/system.hpp"
#include "parent_a.hpp"
#include "parent_b.hpp"


// this include should always be right before the parallel class declaration
#include "child.iface.hpp"

POP_CLASS child : public parent_a, public parent_b
{
	public:
		// POP_ALLOCATION(pop::ssh_allocator("lwinkler@localhost"))
		POP_ALLOCATION(pop::local_allocator())
		child(std::string _str) : str_(_str), parent_a("A:" + _str), parent_b("B:" + _str) {
			LOG(info) << "call child constr";
		}

		void child_method(){
			LOG(info) << "Called a child method from " << str_;
		}

		std::string get_non_virtual_name() {return "child:" + str_;}
		virtual std::string get_virtual_name(){return "child:" + str_;}


	protected:
		const std::string str_ = "Child";
	
};

#endif
