//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_CLIENT_H
#define POP_CLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include "connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include "class/interface.hpp"

namespace pop {

	/// Downloads stock quote information from a server.
	class client
	{
		public:
			/// Constructor starts the asynchronous connect operation.
			client(boost::asio::io_service& io_service, const std::string& host, const std::string& service)
				: connection_(io_service)
			{
				// Resolve the host name into an IP address.
				boost::asio::ip::tcp::resolver resolver(io_service);
				boost::asio::ip::tcp::resolver::query query(host, service);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
					resolver.resolve(query);
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

				// Start an asynchronous connect operation.
				connection_.socket().async_connect(endpoint,
						boost::bind(&client::handle_connect, this,
							boost::asio::placeholders::error, ++endpoint_iterator));
			}

			/// Handle completion of a connect operation.
			void handle_connect(const boost::system::error_code& e,
					boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
			{
				if (!e)
				{
					// Successfully established connection. Start operation to read the list
					// of stocks. The connection::async_read() function will automatically
					// decode the data that is read from the underlying socket.
					int method_id = 1;
				std::cout<<__LINE__<<std::endl;
					connection_.async_write(method_id,
							boost::bind(&client::handle_write0, this,
								boost::asio::placeholders::error));
				}
				else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
				{
					// Try the next endpoint.
					connection_.socket().close();
					boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
					connection_.socket().async_connect(endpoint,
							boost::bind(&client::handle_connect, this,
								boost::asio::placeholders::error, ++endpoint_iterator));
				}
				else
				{
					// An error occurred. Log it and return. Since we are not starting a new
					// operation the io_service will run out of work to do and the client will
					// exit.
					std::cerr << e.message() << std::endl;
				}
			}

			void handle_write0(const boost::system::error_code& e)
			{
					connection_.async_write(tup,
							boost::bind(&client::handle_write, this,
								boost::asio::placeholders::error));
			}

			/// Handle completion of a read operation.
			void handle_write(const boost::system::error_code& e)
			{
				if (!e)
				{
				std::cout<<__LINE__<<std::endl;
					connection_.async_read(tup,
							boost::bind(&client::handle_read, this,
								boost::asio::placeholders::error));
				}
				else
				{
					// An error occurred.
					std::cerr << e.message() << std::endl;
				}

				// Since we are not starting a new operation the io_service will run out of
				// work to do and the client will exit.
			}

			/// Handle completion of a read operation.
			void handle_read(const boost::system::error_code& e)
			{
				if (!e)
				{
					// Print out the data that was received.
					std::cout<<"received "<<std::endl;
					bufout oa(cout);
					oa << tup;
				}
				else
				{
					// An error occurred.
					std::cerr << e.message() << std::endl;
				}

				// Since we are not starting a new operation the io_service will run out of
				// work to do and the client will exit.
			}

		private:
			/// The connection to the server.
			connection connection_;

			/// The data received from the server.
			//interface iface;
			std::tuple<int,int,double,string> tup;
	};

} // namespace

#endif
