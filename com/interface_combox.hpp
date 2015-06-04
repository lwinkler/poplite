//
// interface_combox.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_CLIENT_H
#define POP_CLIENT_H

#include "connection.hpp" // Must come before boost/serialization headers.
#include "accesspoint.hpp"


namespace pop {
	/// The connection class provides serialization primitives on top of a socket.
	class interface_combox
	{
		public:
			/// Constructor.
			interface_combox() :
				acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0 /*port*/))),
				connection_(io_service_)
			{
				// Start an accept operation for a new connection.
				// connection_ptr new_conn();
				LOG(info) << "Listen on port " << acceptor_.local_endpoint().port();
				acceptor_.async_accept(connection_.socket(), boost::bind(&interface_combox::handle_accept, this, boost::asio::placeholders::error));
			}

			inline void run(){io_service_.run();}
			inline connection& connec(){return connection_;}
			inline const boost::asio::ip::tcp::endpoint endpoint() const {return acceptor_.local_endpoint();}
			inline const pop::accesspoint& contact(){return contact_;}

			void send_contact(const boost::asio::ip::tcp::endpoint& _contact_endpoint)
			{
				// Send the address to the broker
				std::stringstream oss;
				bufout oa(oss);
				std::string host_name = acceptor_.local_endpoint().address().to_string();
				std::string service_name = std::to_string(acceptor_.local_endpoint().port());
				oa << host_name;
				oa << service_name;

				connection contact_connection(io_service_);
				contact_connection.sync_write(oss);
			}

		private:
			/// Handle completion of a accept operation.
			void handle_accept(const boost::system::error_code& e)
			{
				if(e)
				{
					throw std::runtime_error(e.message());
				}

				connection_.sync_read();
				bufin ia(connection_.input_stream());
				ia >> contact_;
				LOG(debug) << "Interface combox connected";
			}



		boost::asio::io_service io_service_;
		connection connection_;
		boost::asio::ip::tcp::acceptor acceptor_;
		pop::accesspoint contact_;
	};
} // namespace

#endif
