//
// parent_b.hpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PARENT_B_H
#define PARENT_B_H

// #include "class/interface.hpp"
#include "class/system.hpp"



// this include should always be right before the parallel class declaration
#include "parent_b.iface.hpp"

POP_CLASS parent_b
{
	public:
		POP_ALLOCATION(pop::local_allocator())
		parent_b(std::string _str) : str_(_str) {
			LOG(info) << "call constr of parent_b: " << _str;
		}

		virtual void set_double(double _d){d_ = _d;}
		virtual int get_double(){return d_;}
		// std::string get_non_virtual_name(){return "parent_b:" + str_;}
		// static std::string get_static_class_name_of_parent_b(){return "parent_b";}

	private:
		double d_;
		const std::string str_;
};

#endif
