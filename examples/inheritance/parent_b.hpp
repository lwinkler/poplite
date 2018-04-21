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
#include "parent_a.hpp"



// this include should always be right before the parallel class declaration
#include "parent_b.iface.hpp"

POP_CLASS parent_b : public parent_a
{
	public:
		POP_ALLOCATION(pop::local_allocator())
		parent_b(std::string _str): parent_a("A: " + _str), str_(_str) {
			LOG(info) << "call constr of parent_b: " << _str;
		}

		virtual void set_double(double _d){d_ = _d;}
		virtual int get_double(){return d_;}
		std::string get_non_virtual_name(){return "parent_b:" + str_;}
		virtual std::string get_virtual_name(){return "parent_b:" + str_;}

	private:
		double d_;
		const std::string str_;
};

#endif
