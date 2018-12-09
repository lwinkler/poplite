//
// tester_default.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2018 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef TEST_DEFAULT_VALUES_H
#define TEST_DEFAULT_VALUES_H

#include <string>
#include <vector>

#ifndef _POP_PARSER_ // this accelerates parsing
#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"
#endif

#include "parser/defs.hpp"

// this include should always be right before the parallel class declaration
#include "tester_default.iface.hpp"


POP_CLASS tester_default {

	template<typename T> void check(const T& _a, const T& _b) {
		if(_a != _b) {
			std::stringstream ss;
			ss << _a << " != " << _b;
			throw std::runtime_error(ss.str());
		}
	}

	template<typename T> void check(const std::vector<T>& _a, const std::vector<T>& _b) {
		if(_a != _b) {
			std::stringstream ss;
			ss << "vector value differ: sizes " << _a.size() << " and " << _b.size();
			throw std::runtime_error(ss.str());
		}
	}

public:
	// POP_ALLOCATION(pop::manual_allocator())
	tester_default(const std::string& _string = "my_object", int _int = 5){
		check<std::string>(_string, "my_object");
		check(_int, 5);
	}

	POP_SYNC void test_default_int(int _var = 12) {check(_var, 12);}
	POP_SYNC void test_default_double(double _var = 3.14) {check(_var, 3.14);}
	POP_SYNC void test_default_string(const std::string& _var = "something") {check(_var, std::string("something"));}
	// TODO POP_SYNC void test_default_string(const std::string _var = "something") {check(_var, std::string("something"));}
	POP_SYNC void test_default_vector_int(const std::vector<int>& _var = {1, 2, 3}) {check( _var, {1, 2, 3});}
};

#endif
