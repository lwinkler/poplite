//
// storage.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TEST_MAP_STORAGE_HPP
#define TEST_MAP_STORAGE_HPP

#include <map>

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"

namespace template_ns {

// this include should always be right before the parallel class declaration
#include "template_ns/map_storage.iface.hpp"

template<typename T1, typename T2> POP_CLASS map_storage
{
public:
	POP_ALLOCATION(pop::local_allocator())
	map_storage() {}
	POP_ASYNC void insert(const T1& _index, const T2& _el) {
		map_.insert(std::pair<T1,T2>(_index, _el));
	}
	T2 erase(const T1& _index) {
		auto it = map_.find(_index);
		assert(it != map_.end());
		T2 val = it->second;
		map_.erase(it);
		return val;
	}
	POP_ASYNC void print() const {
		for(const auto& el : map_)
			std::cout << el.first << ":" << el.second << ", ";
		std::cout << std::endl;
	}

protected:
	std::map<T1, T2> map_;
};
} // namespace

#endif
