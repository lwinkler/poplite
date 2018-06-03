//
// storage.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TEST_PROXY_STORAGE_HPP
#define TEST_PROXY_STORAGE_HPP

#include <vector>

#include "storage.hpp"

namespace template_ns {
// this include should always be right before the parallel class declaration
#include "template_ns/proxy_storage.iface.hpp"

template<typename T>
POP_CLASS POP_TEMPLATE_TYPES(<int>;<std::string>)
proxy_storage
{
public:
	POP_ALLOCATION(pop::local_allocator())
	proxy_storage() {}
	POP_ASYNC void push(const T& _el) {
		storage_.push(_el);
	}
	POP_SYNC T pop() {
		return storage_.pop();
	}
	POP_ASYNC void print() {
		storage_.print();
	}

protected:
	storage_iface<T> storage_;
};
} // namespace

#endif
