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
#include <thread>


namespace pop {


	/// Remove server for each parallel object
	template<class ParClass>class broker_combox
	{
		public:
			/// Constructor opens the acceptor and starts waiting for the first incoming
			/// connection.
			broker_combox(remote::broker<ParClass>& _brok, const boost::asio::ip::tcp::resolver::query & _query) :
				brok_(_brok),
				service_acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0 /*port*/))
				// contact_connection_(io_service_)
			{
				boost::asio::ip::tcp::resolver resolver(io_service_);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(_query);
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

				// Start a connection as a point of contact
				connection_ptr service_connection(new connection(io_service_));
				service_acceptor_.async_accept(service_connection->socket(), boost::bind(&broker_combox::handle_accept_service, this, boost::asio::placeholders::error, service_connection));

				// Start an asynchronous connect operation for the connecting interface
				LOG(debug) <<"async connect";
				connection_ptr new_conn(new connection(io_service_));
				new_conn->socket().async_connect(endpoint, boost::bind(&broker_combox::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator, new_conn));
			}
			/// Run io server
			inline void run(){
				std::vector<std::thread> workers;
				// note: so far we can only set up a fixed number of threads
				//       this determines the max number of simultaneous calls to the broker
				for (int i = 0; i < 10; i++)
				{
					workers.push_back(std::thread([&]() {io_service_.run();}));
				}
				std::for_each(workers.begin(), workers.end(), [](std::thread &t){t.join();});
			}

			inline const boost::asio::ip::tcp::endpoint& contact() const {return service_acceptor_.local_endpoint();}

		private:
			/// Handle completion of a connect operation.
			void handle_connect(const boost::system::error_code& e, boost::asio::ip::tcp::resolver::iterator endpoint_iterator, connection_ptr conn)
			{
				if (!e)
				{
					pop::accesspoint ap(service_acceptor_.local_endpoint());
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
				bufin ia(conn->input_stream());
				int method_id = -1;
				ia >> method_id;

				LOG(debug) << "method id " << method_id;

				bool is_async = false;
				ia >> is_async;
				LOG(debug) << "async " << is_async;

				std::stringstream oss;
				bufout oa(oss);
				LOG(debug) << "call remote method " << method_id;

				if(method_id == -1)
				{
					LOG(debug) << "received end signal";
					conn->socket().close();
					// io_service_.stop();
					return;
				}
				else if(is_async)
				{
					// if the call is asynchronous, we send the ack directly

					std::string ack("ACK");
					oa << ack;
					conn->sync_write_ss(oss);
					LOG(debug) << "sent ack";
					brok_.remote_call(method_id, ia, oa);
					LOG(debug) << "finish calling remote method " << method_id;
				}
				else
				{
					brok_.remote_call(method_id, ia, oa);

					LOG(debug) << "finish calling remote method " << method_id;

					std::string ack("ACK");
					oa << ack;
					conn->sync_write_ss(oss);
					LOG(debug) << "sent ack";
				}

				conn->async_read(boost::bind(&broker_combox::handle_read, this, boost::asio::placeholders::error, conn));
			}

			/// Handle contact by a new interface
			void handle_accept_service(const boost::system::error_code& e, connection_ptr service_connection)
			{
				if(e)
				{
					throw std::runtime_error(e.message());
				}
				service_connection->sync_read();
				bufin ia(service_connection->input_stream());
				int service_type = -1;
				ia >> service_type;
				LOG(debug) << "Interface combox contacted";
				switch(service_type)
				{
					case 0:
						// Closing the service:
						LOG(debug) << "Closing service on broker";
						return;
					case 1: // TODO ENUM
					{
						// Open a contact with a new interface
						pop::accesspoint ap;
						ia >> ap;
						LOG(debug) << "Call iface on "<< ap.host_name << " " << ap.port;

						boost::asio::ip::tcp::resolver resolver(io_service_);
						boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(ap.create_query());
						boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

						// Start an asynchronous connect operation and connect to interface
						LOG(debug) <<"async connect";
						connection_ptr new_conn(new connection(io_service_));
						new_conn->socket().async_connect(endpoint, boost::bind(&broker_combox::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator, new_conn));
					}
					break;
					default:
						throw std::runtime_error("Unknown service_type " + std::to_string(service_type));
				}

				// Recreate a connection for contact
				connection_ptr new_conn2(new connection(io_service_));
				service_acceptor_.async_accept(new_conn2->socket(), boost::bind(&broker_combox::handle_accept_service, this, boost::asio::placeholders::error, new_conn2));
			}

		private:
			/// The data to be sent to each client.
			pop::remote::broker<ParClass>& brok_;
			boost::asio::io_service io_service_;
			boost::asio::ip::tcp::acceptor service_acceptor_;
	};

} // namespace 

#endif
