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

#include <iostream>
#include <string>

#ifndef _POP_PARSER_ // this accelerates parsing
#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"
#endif

#include "parser/defs.hpp"

namespace global_ns {

namespace simple_parent_a_ns {
class simple_parent_a {
public:
	POP_SYNC void set_float_a(float _f) {
		f_ = _f;
	}
	POP_SYNC float get_float_a() {
		return f_;
	}
	POP_SYNC virtual float more_magic() {
		return 123e2;
	}
	POP_SYNC virtual short pure_virtual() const = 0;
private:
	float f_ = 0;
};
} // namespace

// this include should always be right before the parallel class declaration
#include "global_ns/parent_a.iface.hpp"
POP_CLASS parent_a :
public simple_parent_a_ns::simple_parent_a {
public:
	POP_ALLOCATION(pop::local_allocator())
	parent_a(std::string _str) : str_(_str) {
		LOG(info) << "call constr of parent_a: " << _str;
	}

	POP_SYNC virtual void set_int(int _i) {
		i_ = _i;
	}
	POP_SYNC virtual int get_int() {
		return i_;
	}
	POP_SYNC std::string get_non_virtual_name() {
		return "parent_a:" + str_;
	}
	POP_SYNC virtual std::string get_virtual_name() {
		return "parent_a:" + str_;
	}
	POP_SYNC static std::string get_static_name() {
		return "parent_a";
	}
	virtual short pure_virtual() const override {return 55*2;};

private:
	int i_;
	const std::string str_;
};



namespace parent_b_ns {
// this include should always be right before the parallel class declaration
#include "global_ns/parent_b_ns/parent_b.iface.hpp"
POP_CLASS parent_b :
public parent_a {
public:
	POP_ALLOCATION(pop::local_allocator())
	parent_b(std::string _str): parent_a("A: " + _str), str_(_str) {
		LOG(info) << "call constr of parent_b: " << _str;
	}

	POP_SYNC virtual void set_double(double _d) {
		d_ = _d;
	}
	POP_SYNC virtual int get_double() {
		return d_;
	}
	POP_SYNC std::string get_non_virtual_name() {
		return "parent_b:" + str_;
	}
	virtual std::string get_virtual_name() {
		return "parent_b:" + str_;
	}
	POP_SYNC static std::string get_static_name() {
		return "parent_b";
	}
	virtual float more_magic() {
		return 456e2;
	}

	virtual short pure_virtual() const override {return 55*4;};

private:
	double d_;
	const std::string str_;
};
} // namespace

class simple_parent_c {
public:
	POP_SYNC void set_float(float _f) {
		f_ = _f;
	}
	POP_SYNC float get_float() {
		return f_;
	}
private:
	float f_ = 0;
};

// this include should always be right before the parallel class declaration
namespace child_ns {
#include "global_ns/child_ns/child.iface.hpp"
POP_CLASS child :
public parent_b_ns::parent_b, public simple_parent_c {
public:
	// POP_ALLOCATION(pop::ssh_allocator("lwinkler@localhost"))
	POP_ALLOCATION(pop::local_allocator())
	child(std::string _str) : str_(_str), parent_b("B:" + _str) {
		LOG(info) << "call child constr";
	}

	POP_SYNC void child_method() {
		LOG(info) << "Called a child method from " << str_;
	}

	POP_SYNC std::string get_non_virtual_name() {
		return "child:" + str_;
	}
	POP_SYNC std::string get_virtual_name() {
		return "child:" + str_;
	}
	POP_SYNC static std::string get_static_name() {
		return "child";
	}

	virtual short pure_virtual() const override {return 55*5;};

protected:
	const std::string str_ = "Child";

};
} // namespace
} // namespace

#endif
