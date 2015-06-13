//
// broker_combox.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Based on examples of 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_BROKER_COMBOX_H
#define POP_BROKER_COMBOX_H

#include "connection.hpp" // Must come before boost/serialization headers.
#include "class/broker.hpp"
#include "com/accesspoint.hpp"


namespace pop {


	/// Serves stock quote information to any client that connects to it.
	template<class ParClass>class broker_combox
	{
		public:
			/// Constructor opens the acceptor and starts waiting for the first incoming
			/// connection.
			broker_combox(remote::broker<ParClass>& _brok, const boost::asio::ip::tcp::resolver::query & _query) :
				brok_(_brok),
				contact_acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0 /*port*/))
				// contact_connection_(io_service_)
			{
				boost::asio::ip::tcp::resolver resolver(io_service_);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(_query);
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

				// Start a connection as a point of contact
				connection_ptr contact_connection(new connection(io_service_));
				contact_acceptor_.async_accept(contact_connection->socket(), boost::bind(&broker_combox::handle_accept_contact, this, boost::asio::placeholders::error, contact_connection));

				// Start an asynchronous connect operation for the connecting interface
				LOG(debug) <<"async connect";
				connection_ptr new_conn(new connection(io_service_));
				new_conn->socket().async_connect(endpoint, boost::bind(&broker_combox::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator, new_conn));

				io_service_.run();
			}
			/// Run io server
			inline void run(){io_service_.run();}
			inline const boost::asio::ip::tcp::endpoint& contact() const {return contact_acceptor_.local_endpoint();}

		private:
			/// Handle completion of a connect operation.
			void handle_connect(const boost::system::error_code& e, boost::asio::ip::tcp::resolver::iterator endpoint_iterator, connection_ptr conn)
			{
				if (!e)
				{
					pop::accesspoint ap(contact_acceptor_.local_endpoint());
					LOG(debug) << "send contact from broker"; // << ap;
					conn->sync_write(ap);

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
				conn->async_read(boost::bind(&broker_combox::handle_read, this, boost::asio::placeholders::error, conn));
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
				bufin ia(conn->input_stream());
				int method_id = -1;
				ia >> method_id;

				LOG(debug) << "method id " << method_id;

				std::stringstream oss;
				bufout oa(oss);
				LOG(debug) << "call remote method " << method_id;

				if(method_id == -1)
					quit = true;
				else
					brok_.remote_call(method_id, ia, oa);

				LOG(debug) << "finish calling remote method " << method_id;

				std::string ack("ACK");
				oa << ack;
				conn->sync_write_ss(oss);
				LOG(debug) << "sent ack";

				if(quit)
				{
					LOG(debug) << "received end signal";
					conn->socket().close();
					io_service_.stop();
					return;
				}
				conn->async_read(boost::bind(&broker_combox::handle_read, this, boost::asio::placeholders::error, conn));
			}

			/// Handle contact by a new interface
			void handle_accept_contact(const boost::system::error_code& e, connection_ptr contact_connection)
			{
				if(e)
				{
					throw std::runtime_error(e.message());
				}
				LOG(debug) << "Interface combox contacted";
				pop::accesspoint ap;
				contact_connection->sync_read(ap);
				LOG(debug) << "Call iface on "<< ap.host_name_ << " " << ap.port_;

				boost::asio::ip::tcp::resolver resolver(io_service_);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(ap.create_query());
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

				// Start an asynchronous connect operation and connect to interface
				LOG(debug) <<"async connect";
				connection_ptr new_conn(new connection(io_service_));
				new_conn->socket().async_connect(endpoint, boost::bind(&broker_combox::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator, new_conn));

				// Recreate a connection for contact
				// TODO: is this possible to have a permanent service for contact ?
				connection_ptr new_conn2(new connection(io_service_));
				contact_acceptor_.async_accept(new_conn2->socket(), boost::bind(&broker_combox::handle_accept_contact, this, boost::asio::placeholders::error, new_conn2));
			}

		private:
			/// The data to be sent to each client.
			pop::remote::broker<ParClass>& brok_;
			boost::asio::io_service io_service_;
			boost::asio::ip::tcp::acceptor contact_acceptor_;
	};

} // namespace 

#endif
