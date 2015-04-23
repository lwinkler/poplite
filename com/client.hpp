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

#include "connection.hpp" // Must come before boost/serialization headers.


namespace pop {
	/// The connection class provides serialization primitives on top of a socket.
	/**
	 * Each message sent using this class consists of:
	 * @li An 8-byte header containing the length of the serialized data in
	 * hexadecimal.
	 * @li The serialized data.
	 */
	class client
	{
		public:
			/// Constructor.
			client(const boost::asio::ip::tcp::resolver::query& query) :
				connection_(io_service_),
				query_(query)
			{
				boost::asio::ip::tcp::resolver resolver(io_service_);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

				// Start an asynchronous connect operation.
				LOG(debug) <<"async connect";
				connection_.socket().async_connect(endpoint, boost::bind(&client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator));
				io_service_.run();
			}

			inline void run(){io_service_.run();}
			inline connection& connec(){return connection_;}


		private:
			/// Handle completion of a connect operation.
			void handle_connect(const boost::system::error_code& e, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
			{
				if (!e)
				{
					LOG(debug)<<"connected";
				}
				else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
				{
					// Try the next endpoint.
					connection_.socket().close();
					boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
					connection_.socket().async_connect(endpoint, boost::bind(&client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator));
				}
				else
				{
					LOG(error) << "connection failed: " << e.message();
				}
			}

			boost::asio::io_service io_service_;
			connection connection_;
			const boost::asio::ip::tcp::resolver::query& query_;
	};
} // namespace

#endif
