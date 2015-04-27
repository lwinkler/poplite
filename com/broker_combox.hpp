//
// broker_combox.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_CLIENT_H
#define POP_CLIENT_H

#include "class/util.hpp"
#include "connection.hpp" // Must come before boost/serialization headers.

#include "class/broker.hpp"


namespace pop {

	/// Serves stock quote information to any client that connects to it.
	template<class ParClass>
		class broker_combox
		{
			public:
				/// Constructor opens the acceptor and starts waiting for the first incoming
				/// connection.
				broker_combox(boost::asio::io_service& io_service, pop::remote::broker<ParClass>& brok, const boost::asio::ip::tcp::resolver::query & query)
					: brok_(brok)
			{
				boost::asio::ip::tcp::resolver resolver(io_service);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

				// Start an asynchronous connect operation.
				LOG(debug) <<"async connect";
				connection_ptr new_conn(new connection(io_service));
				new_conn->socket().async_connect(endpoint, boost::bind(&broker_combox::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator, new_conn));
				io_service.run();
			}
			/// Handle completion of a connect operation.
			void handle_connect(const boost::system::error_code& e, boost::asio::ip::tcp::resolver::iterator endpoint_iterator, connection_ptr conn)
			{
				if (!e)
				{
					LOG(debug)<<"connected";
				}
				else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
				{
					// Try the next endpoint.
					conn->socket().close();
					boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
					conn->socket().async_connect(endpoint, boost::bind(&broker_combox::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator, conn));
				}
				else
				{
					throw std::runtime_error("connection failed");
				}

				conn->async_read(conn->method_id, boost::bind(&broker_combox::handle_read, this, boost::asio::placeholders::error, conn));
			}


			void handle_read(const boost::system::error_code& e, connection_ptr conn)
			{
				if(e)
				{
					throw std::runtime_error("handle_read: " + e.message());
				}
				// Receive an incomming remote method invocation


				// Successfully accepted a new connection. Send the list of stocks to the
				// client. The connection::async_write() function will automatically
				// serialize the data structure for us.
				// conn->async_read(stocks_, boost::bind(&interface_combox::handle_read, this, boost::asio::placeholders::error, conn));
				std::cout << __LINE__ << std::endl;
				// int method_id = -1;
				LOG(debug) << "method id " << conn->method_id;

				std::stringstream ss2;
				conn->sync_read(ss2);
				bufin ia2(ss2);

				std::stringstream ss3;
				bufout oa(ss3);
				LOG(debug) << "call remote method " << conn->method_id;
				brok_.remote_call(conn->method_id, ia2, oa);
				LOG(debug) << "finish calling remote method " << conn->method_id;
				conn->sync_write(ss3);

				std::string ack("ACK");
				std::stringstream ss4;
				bufout oa2(ss4);
				oa2 << ack;
				conn->sync_write(ss4);
				LOG(debug) << "send ack";

				conn->async_read(conn->method_id, boost::bind(&broker_combox::handle_read, this, boost::asio::placeholders::error, conn));
			}

/*
			/// Handle completion of a write operation.
			void handle_read(const boost::system::error_code& e, connection_ptr conn)
			{
				std::tuple<int,int,double,std::string> tup;

				std::stringstream ss;
				bufout oa(ss);
				oa << tup;
				LOG(debug) << "send data";
				conn->sync_write(ss);
				boost::system::error_code error;

				// LOG(debug) << "create new connection";
				// connection_ptr new_conn(new connection(acceptor_.get_io_service()));
				// acceptor_.async_accept(new_conn->socket(), boost::bind(&broker_combox::handle_accept, this, boost::asio::placeholders::error, new_conn));
			}
			*/

		private:
			/// The acceptor object used to accept incoming socket connections.

			/// The data to be sent to each client.
			// std::vector<stock> stocks_;
			pop::remote::broker<ParClass>& brok_;
	};

} // namespace s11n_example

#endif
