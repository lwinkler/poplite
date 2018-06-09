//
// mutex.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_MUTEX_HPP
#define POP_MUTEX_HPP

#include <iostream>
#include <boost/signals2/mutex.hpp>

#ifndef _POP_PARSER_ // this accelerates parsing
#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"
#endif

#include "parser/defs.hpp"

namespace pop {

// this include should always be right before the parallel class declaration
#include "pop/mutex.iface.hpp"

/// A simple class that calls another object
POP_CLASS mutex {
public:
	mutex() {
		mtx_.lock();
	}

	~mutex() {
		mtx_.lock();
		mtx_.unlock();
	}

	void POP_ASYNC unlock() {
		LOG(debug) << "Unlock mutex";
		mtx_.unlock();
	}

	void POP_SYNC wait() {
		LOG(debug) << "Wait in mutex";
		mtx_.lock();
		mtx_.unlock();
		LOG(debug) << "Stop waiting in mutex";
	}

private:
	boost::mutex mtx_;
};
} // namespace pop

#endif
