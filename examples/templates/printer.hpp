//
// printer.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TEST_PRINTER_HPP
#define TEST_PRINTER_HPP

#include <iostream>

// #ifndef _POP_PARSER_ // this accelerates parsing
#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"
// #endif

#include "parser/defs.hpp"

namespace template_ns {

// this include should always be right before the parallel class declaration
#include "template_ns/printer.iface.hpp"

POP_CLASS printer {
public:
	POP_ALLOCATION(pop::local_allocator())
	printer() {}
	template<typename T> POP_ASYNC POP_TEMPLATE_TYPES(<int>; <std::string>)
	void print(const T& _elem) const {
		std::cout << _elem << std::endl;
	}
	POP_SYNC std::string name() const {return "printer";}

	template<typename T> POP_ASYNC POP_TEMPLATE_TYPES(<int>; <std::string>)
	static void static_print(const T& _elem) {
		std::cout << _elem << std::endl;
	}

	template<typename T> POP_ASYNC POP_TEMPLATE_TYPES(<int>; <std::string>)
	void const_print(const T& _elem) const {
		std::cout << _elem << std::endl;
	}

};
} // namespace

#endif
