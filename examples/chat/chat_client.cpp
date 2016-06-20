//
// chat_client.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "chat_client.hpp"
#include "chat_client.iface.hpp"

void chat_client::send(const std::string& line_)
{
	for(auto& elem : contacts_)
	{
		chat_client_iface cl(elem);
		cl.write(line_);
	}
}
