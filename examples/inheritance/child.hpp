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
#include "class/interface.hpp"
#include "alloc/local.hpp"

class simple_parent_a {
	public:
		void set_float_a(float _f){f_ = _f;}
		float get_float_a(){return f_;}
		virtual float more_magic(){return 123e2;}
	private:
		float f_ = 0;
};

POP_CLASS parent_a : public simple_parent_a
{
	public:
		#include "parent_a.iface.hpp"

		POP_ALLOCATION(pop::local_allocator())
		parent_a(std::string _str) : str_(_str) {
			LOG(info) << "call constr of parent_a: " << _str;
		}

		virtual void set_int(int _i){i_ = _i;}
		virtual int get_int(){return i_;}
		std::string get_non_virtual_name(){return "parent_a:" + str_;}
		virtual std::string get_virtual_name(){return "parent_a:" + str_;}
		static std::string get_static_name(){return "parent_a";}

	private:
		int i_;
		const std::string str_;
};


POP_CLASS parent_b : public parent_a
{
	public:
		#include "parent_b.iface.hpp"

		POP_ALLOCATION(pop::local_allocator())
		parent_b(std::string _str): parent_a("A: " + _str), str_(_str) {
			LOG(info) << "call constr of parent_b: " << _str;
		}

		virtual void set_double(double _d){d_ = _d;}
		virtual int get_double(){return d_;}
		std::string get_non_virtual_name(){return "parent_b:" + str_;}
		virtual std::string get_virtual_name(){return "parent_b:" + str_;}
		static std::string get_static_name(){return "parent_b";}
		virtual float more_magic(){return 456e2;}

	private:
		double d_;
		const std::string str_;
};

class simple_parent_c {
	public:
		void set_float(float _f){f_ = _f;}
		float get_float(){return f_;}
	private:
		float f_ = 0;
};

POP_CLASS child : public parent_b, public simple_parent_c
{
	public:
		#include "child.iface.hpp"
		
		// POP_ALLOCATION(pop::ssh_allocator("lwinkler@localhost"))
		POP_ALLOCATION(pop::local_allocator())
		child(std::string _str) : str_(_str), parent_b("B:" + _str) {
			LOG(info) << "call child constr";
		}

		void child_method(){
			LOG(info) << "Called a child method from " << str_;
		}

		std::string get_non_virtual_name() {return "child:" + str_;}
		std::string get_virtual_name(){return "child:" + str_;}
		static std::string get_static_name(){return "child";}

	protected:
		const std::string str_ = "Child";
	
};

#endif
