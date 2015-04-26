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


namespace pop {
	/// The connection class provides serialization primitives on top of a socket.
	/**
	 * Each message sent using this class consists of:
	 * @li An 8-byte header containing the length of the serialized data in
	 * hexadecimal.
	 * @li The serialized data.
	 */
	class interface_combox
	{
		public:
			/// Constructor.
			interface_combox(boost::asio::ip::tcp::endpoint& _endpoint) :
				acceptor_(io_service_, _endpoint),
				connection_(io_service_)
			{
				// Start an accept operation for a new connection.
				connection_ptr new_conn(new connection(acceptor_.get_io_service()));
				LOG(info) << "Listen on port " << acceptor_.local_endpoint().port();
				acceptor_.async_accept(new_conn->socket(), boost::bind(&interface_combox::handle_accept, this, boost::asio::placeholders::error, new_conn));

				// Handle connection
				io_service_.run();
			}

			inline void run(){io_service_.run();}
			inline connection& connec(){return connection_;}


		private:
			/// Handle completion of a accept operation.
			void handle_accept(const boost::system::error_code& e, connection_ptr conn)
			{
				if(e)
				{
					throw std::runtime_error(e.message());
				}
				LOG(debug) << "Interface combox connected";
			}


		boost::asio::io_service io_service_;
		connection connection_;
		boost::asio::ip::tcp::acceptor acceptor_;
	};
} // namespace

#endif
