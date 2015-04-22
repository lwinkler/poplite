//
// server.cpp
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
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include "connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>

#include "class/broker.hpp"
#include "example/test_class.hpp"


namespace pop {

	/// Serves stock quote information to any client that connects to it.
	template<class ParClass>
	class server
	{
		public:
			/// Constructor opens the acceptor and starts waiting for the first incoming
			/// connection.
			server(boost::asio::io_service& io_service, unsigned short port, pop::remote::broker<ParClass>& brok)
				: acceptor_(io_service,	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
				m_brok(brok)
		{
			// Start an accept operation for a new connection.
			connection_ptr new_conn(new connection(acceptor_.get_io_service()));
			acceptor_.async_accept(new_conn->socket(),
					boost::bind(&server::handle_accept, this,
						boost::asio::placeholders::error, new_conn));
		}

			/// Handle completion of a accept operation.
			void handle_accept(const boost::system::error_code& e, connection_ptr conn)
			{
				if (!e)
				{
					// Successfully accepted a new connection. Send the list of stocks to the
					// client. The connection::async_write() function will automatically
					// serialize the data structure for us.
#if ASYNC 
					conn->async_read(stocks_,
							boost::bind(&server::handle_read, this,
								boost::asio::placeholders::error, conn));
#else
					int method_id = -1;
					std::stringstream ss;
					conn->sync_read(ss);
					// cout << oa << endl;
					pop::bufin ia(ss);
					ia >> method_id;
					
				std::tuple<int,int,double,string> tup;
					cout << "mid" << method_id << endl;

					std::stringstream ss2;
					conn->sync_read(ss2);
					bufin ia2(ss2);
					ia2 >> tup;
					cout << "received tuple" << endl;
					boost::system::error_code error;
					handle_read(error, conn);
#endif


				}
				else
				{
					// An error occurred. Log it and return. Since we are not starting a new
					// accept operation the io_service will run out of work to do and the
					// server will exit.
					std::cerr << e.message() << std::endl;
				}
			}

			/// Handle completion of a write operation.
			void handle_read(const boost::system::error_code& e, connection_ptr conn)
			{
				// Nothing to do. The socket will be closed automatically when the last
				// reference to the connection object goes away.
					
				std::tuple<int,int,double,string> tup;

#ifdef ASYNC
					conn->async_write(tup,
							boost::bind(&server::handle_write, this,
								boost::asio::placeholders::error, conn));

					// Start an accept operation for a new connection.
					connection_ptr new_conn(new connection(acceptor_.get_io_service()));
					acceptor_.async_accept(new_conn->socket(),
							boost::bind(&server::handle_accept, this,
								boost::asio::placeholders::error, new_conn));
#else
					std::stringstream ss;
					bufout oa(ss);
					oa << tup;
					std::cout << "send tuple " << std::endl;
					conn->sync_write(ss);
					std::cout << "sent tuple " << std::endl;
					boost::system::error_code error;
					handle_write(error, conn);

					connection_ptr new_conn(new connection(acceptor_.get_io_service()));
					acceptor_.async_accept(new_conn->socket(),
							boost::bind(&server::handle_accept, this,
								boost::asio::placeholders::error, new_conn));
#endif

			}

			/// Handle completion of a write operation.
			void handle_write(const boost::system::error_code& e, connection_ptr conn)
			{
				// Nothing to do. The socket will be closed automatically when the last
				// reference to the connection object goes away.
			}

		private:
			/// The acceptor object used to accept incoming socket connections.
			boost::asio::ip::tcp::acceptor acceptor_;

			/// The data to be sent to each client.
			// std::vector<stock> stocks_;
			pop::remote::broker<ParClass>& m_brok;
	};

} // namespace s11n_example

#endif
