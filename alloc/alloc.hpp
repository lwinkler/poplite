//
// alloc.hpp
// ~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_ALLOC_H
#define POP_ALLOC_H

#include <boost/asio/ip/tcp.hpp>
#include "com/accesspoint.hpp"

namespace pop {

/// Parent class for all allocators
/// allocators can launch an object on a local or remote platform

class allocator
{
public:
	/// Allocate an object at endpoint
	virtual void allocate(const std::string& _executable, const std::string& _class_name, const pop::accesspoint& _callback) const = 0;
};
}

#endif
