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

namespace pop{

/// Class to allocate object manually: the command has to be run by the user on any platform
class manual_allocator : public allocator
{
	public:
	manual_allocator(){}
	void allocate(const std::string& _obj_name, const boost::asio::ip::tcp::endpoint& _endpoint) const
	{
		std::stringstream ss;
		ss << "./" << _obj_name << " " << _endpoint.address() << " " << _endpoint.port();
		LOG(info) << "Start object " << _obj_name << " with command :";
		LOG(info) << ss.str();
	}
};
}

#endif
