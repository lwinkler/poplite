//
// parent_a.hpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PARENT_A_H
#define PARENT_A_H

// #include "class/interface.hpp"
#include "class/system.hpp"

class simple_parent_a {
	public:
		void set_float_a(float _f){f_ = _f;}
		float get_float_a(){return f_;}
	private:
		float f_ = 0;
};

// this include should always be right before the parallel class declaration
#include "parent_a.iface.hpp"

POP_CLASS parent_a : public simple_parent_a
{
	public:
		POP_ALLOCATION(pop::local_allocator())
		parent_a(std::string _str) : str_(_str) {
			LOG(info) << "call constr of parent_a: " << _str;
		}

		virtual void set_int(int _i){i_ = _i;}
		virtual int get_int(){return i_;}
		std::string get_non_virtual_name(){return "parent_a:" + str_;}
		virtual std::string get_virtual_name(){return "parent_a:" + str_;}
		// static std::string get_static_class_name_of_parent_a(){return "parent_a";} // TODO

	private:
		int i_;
		const std::string str_;
};

#endif
