//
// interface_combox.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Based on examples of 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
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
			/// Constructor
			interface_combox() :
				acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0 /*port*/))),
				connection_(io_service_)
			{
				// Start an accept operation for a new connection.
				// connection_ptr new_conn();
				LOG(debug) << "Listen on port " << acceptor_.local_endpoint().port();
				acceptor_.async_accept(connection_.socket(), boost::bind(&interface_combox::handle_accept, this, boost::asio::placeholders::error));
			}
			~interface_combox(){io_service_.stop();}

			inline void run(){io_service_.run();}
			inline connection& connec(){return connection_;}
			inline const boost::asio::ip::tcp::endpoint endpoint() const {return acceptor_.local_endpoint();}
			inline const pop::accesspoint& contact(){return contact_;}

			void send_my_contact(const pop::accesspoint& _contact, const pop::accesspoint& _to)
			{
				boost::asio::ip::tcp::resolver resolver(io_service_);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(_to.create_query());

				connection contact_connection(io_service_);
				boost::asio::connect(contact_connection.socket(), endpoint_iterator);

				// Send the address to the broker
				contact_connection.sync_write(_contact);
			}

		private:
			/// Handle completion of a accept operation
			void handle_accept(const boost::system::error_code& e)
			{
				if(e)
				{
					throw std::runtime_error(e.message());
				}

				connection_.sync_read(contact_);
				LOG(debug) << "Interface combox connected";
			}



		boost::asio::io_service io_service_;
		connection connection_;
		boost::asio::ip::tcp::acceptor acceptor_;
		pop::accesspoint contact_;
	};
} // namespace

#endif
