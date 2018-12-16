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
	using Brok = pop::remote::broker<T, pop::remote::broker_constructor_sync<T>>;
public:
	local_broker() : 
		brok_(new T()),
		combox_(brok_)
	{
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
	Brok brok_;
	pop::broker_combox<Brok> combox_;
};
} // namespace pop

#endif
