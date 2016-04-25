//
// chat_client.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHAT_CLIENT_HPP
#define CHAT_CLIENT_HPP

#include "class/system.hpp"
#include "class/interface.hpp"

POP_CLASS chat_client
{
	public:
		// POP_ALLOCATION(pop::ssh_allocator("lwinkler@192.168.178.34"))
		POP_ALLOCATION(pop::local_allocator())
		chat_client() {std::cout << "Create chat client locally" << std::endl;}

		void POP_SYNC write(const std::string& line_)
		{
			std::cout << line_ << std::endl;
		}

		void POP_SYNC connect(const pop::accesspoint& _ap)
		{
			contacts_.push_back(_ap);
		}

		void POP_SYNC send(const std::string& line_);

	private:
		std::vector<pop::accesspoint> contacts_;
};

#endif
