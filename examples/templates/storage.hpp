//
// storage.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TEST_STORAGE_HPP
#define TEST_STORAGE_HPP

#include <vector>

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"

namespace template_ns {

// this include should always be right before the parallel class declaration
#include "template_ns/storage.iface.hpp"

template<typename T> POP_CLASS storage
{
	public:
		POP_ALLOCATION(pop::local_allocator())
		storage() {}
		POP_ASYNC void push(const T& _el) {stack_.push_back(_el);}
		T pop() {
			T val = stack_.back();
			stack_.pop_back();
			return val;
		}
		POP_ASYNC void print() const {
			for(const auto& el : stack_)
				std::cout << el << std::endl;
		}
	
	protected:
		std::vector<T> stack_;
};
} // namespace

#endif
