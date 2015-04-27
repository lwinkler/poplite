//
// broker_combox.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_BROKER_COMBOX_H
#define POP_BROKER_COMBOX_H

#include "class/util.hpp"
#include "connection.hpp" // Must come before boost/serialization headers.

#include "class/broker.hpp"


namespace pop {


	/// Serves stock quote information to any client that connects to it.
	template<class ParClass>class broker_combox
	{
		public:
			/// Constructor opens the acceptor and starts waiting for the first incoming
			/// connection.
			broker_combox(pop::remote::broker<ParClass>& _brok, const boost::asio::ip::tcp::resolver::query & _query)
				: brok_(_brok)
			{
				boost::asio::ip::tcp::resolver resolver(io_service_);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(_query);
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

				// Start an asynchronous connect operation.
				LOG(debug) <<"async connect";
				connection_ptr new_conn(new connection(io_service_));
				new_conn->socket().async_connect(endpoint, boost::bind(&broker_combox::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator, new_conn));
				io_service_.run();
			}
			/// Run io server
			inline void run(){io_service_.run();}

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
				// Successfully accepted a new connection. Call method by id
				bool quit = false;
				LOG(debug) << "method id " << conn->method_id;

				std::stringstream ss2;
				conn->sync_read(ss2);
				bufin ia2(ss2);

				std::stringstream ss3;
				bufout oa(ss3);
				LOG(debug) << "call remote method " << conn->method_id;

				if(conn->method_id == -1)
					quit = true;
				else
					brok_.remote_call(conn->method_id, ia2, oa);

				LOG(debug) << "finish calling remote method " << conn->method_id;
				conn->sync_write(ss3);

				// TODO: probably not needed
				std::string ack("ACK");
				std::stringstream ss4;
				bufout oa2(ss4);
				oa2 << ack;
				conn->sync_write(ss4);
				LOG(debug) << "send ack";

				if(quit)
				{
					LOG(debug) << "received end signal";
					conn->socket().close();
					return;
				}
				conn->async_read(conn->method_id, boost::bind(&broker_combox::handle_read, this, boost::asio::placeholders::error, conn));
			}

		private:
			/// The data to be sent to each client.
			pop::remote::broker<ParClass>& brok_;
			boost::asio::io_service io_service_;
	};

} // namespace s11n_example

#endif
