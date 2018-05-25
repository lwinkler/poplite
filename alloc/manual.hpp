//
// manual.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_ALLOC_MANUAL_H
#define POP_ALLOC_MANUAL_H

#include "class/system.hpp"
#include "alloc/alloc.hpp"

namespace pop {

/// Class to allocate object manually: the command has to be run by the user on any platform
class manual_allocator : public allocator
{
	public:
	manual_allocator(){}
	void allocate(const std::string& _executable, const std::string& _class_name, const pop::accesspoint& _callback) const
	{
		std::stringstream ss;
		ss << "./" << _executable << " '" << _class_name << "'" << " " << _callback.host_name << " " << _callback.port;
		pop::system::instance().print_args(ss);
		LOG(info) << "Start object " << _executable << " with command :";
		LOG(info) << ss.str();
	}
};
}

#endif
