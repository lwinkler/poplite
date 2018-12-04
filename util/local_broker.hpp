// local_broker.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_LOCAL_BROKER_HPP
#define POP_LOCAL_BROKER_HPP

#include <iostream>
#include "class/broker.hpp"
#include "com/broker_combox.hpp"

/// A template to create objects locally, without interface or dedicated process (see ping_pong for an example)
namespace pop {
template<typename T> class local_broker final {
public:
	local_broker() : combox_(brok_), brok_(new T()) {
	}

	inline void run() {
		combox_.run();
	}

	inline void stop() {
		combox_.stop();
	}

	inline T& object() {
		return brok_.obj();
	}

	inline const pop::accesspoint& contact() const {
		return combox_.contact();
	}
private:
	pop::broker_combox<T> combox_;
	pop::remote::broker<T> brok_;
};
} // namespace pop

#endif
