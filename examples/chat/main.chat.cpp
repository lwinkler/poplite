//
// main.chat.cpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include "chat_client.iface.hpp"

using namespace std;

/// A simple chat example for poplite

int main(int argc, char* argv[])
{
	try
	{
		chat_client_iface cl1;
		chat_client_iface cl2;
		cl1.connect(cl2.contact());
		cl2.connect(cl1.contact());
		cl1.send("This is a test message");
		cl2.send("This is another test message");
	}
	catch (std::exception& e)
	{
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}

	return 0;
}
