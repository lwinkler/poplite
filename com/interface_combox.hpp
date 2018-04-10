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

#include "connection.hpp"
#include "accesspoint.hpp"


namespace pop {
	/// The connection class provides serialization primitives on top of a socket.
	class interface_combox
	{
		public:
			/// Constructor
			interface_combox() :
				acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0 /*port*/))),
				callback_(pop::system::instance().host_name(), acceptor_.local_endpoint().port()),
				connection_(io_service_)
			{
				// Start an accept operation for a new connection.
				// connection_ptr new_conn();
				LOG(debug) << "Listen on port " << acceptor_.local_endpoint().port();
				acceptor_.async_accept(connection_.socket(), boost::bind(&interface_combox::handle_accept, this, boost::asio::placeholders::error));
			}
			interface_combox(interface_combox&&) = default;
			interface_combox& operator=(interface_combox&&) = default;

			// you shall not copy
			interface_combox(const interface_combox&) = delete;
			interface_combox& operator=(const interface_combox&) = delete;

			// virtual ~interface_combox(){/*io_service_.stop();*/}

			inline void run(){io_service_.run();}
			inline connection& connec(){return connection_;}
			inline const boost::asio::ip::tcp::endpoint endpoint() const {return acceptor_.local_endpoint();}
			inline const pop::accesspoint& contact() const {return contact_;}
			inline const pop::accesspoint& callback() const {return callback_;}

			void send_my_contact(const pop::accesspoint& _to)
			{
				boost::asio::ip::tcp::resolver resolver(io_service_);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(_to.create_query());

				connection contact_connection(io_service_);
				boost::asio::connect(contact_connection.socket(), endpoint_iterator);

				// Send the address to the broker
				std::stringstream oss;
				bufout oa(oss);
				static const int service_type = 1; // TODO enum
				oa << service_type;
				pop::accesspoint ap(pop::system::instance().host_name(), acceptor_.local_endpoint().port());
				oa << ap;
				contact_connection.sync_write_ss(oss);
			}

			void close_service()
			{
				LOG(debug) << "Closing service from interface";
				boost::asio::ip::tcp::resolver resolver(io_service_);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(contact_.create_query());

				connection contact_connection(io_service_);
				boost::asio::connect(contact_connection.socket(), endpoint_iterator);

				// Send the address to the broker
				std::stringstream oss;
				bufout oa(oss);
				static const int service_type = 0; // TODO enum
				oa << service_type;
				contact_connection.sync_write_ss(oss);
			}

		private:
			/// Handle completion of a accept operation
			void handle_accept(const boost::system::error_code& e)
			{
				if(e)
				{
					LOG(error) << "Error code in handle_accept: " << e.message();
					throw std::runtime_error(e.message());
				}

				connection_.sync_read(contact_);
				LOG(debug) << "Interface combox connected";
			}



		boost::asio::io_service io_service_;
		boost::asio::ip::tcp::acceptor acceptor_;
		pop::accesspoint callback_;
		connection connection_;
		pop::accesspoint contact_;
	};
} // namespace

#endif
