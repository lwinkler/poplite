//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POP_CONNECTION_HPP
#define POP_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#include "com/serialize.hpp"

using namespace std;

namespace pop {

	class connection
	{
		public:
			connection(boost::asio::io_service& io_service)
				: socket_(io_service)
				{}


			/// Get the underlying socket. Used for making a connection or for accepting
			/// an incoming connection.
			inline boost::asio::ip::tcp::socket& socket()
			{
				return socket_;
			}

			/// Asynchronously write a data structure to the socket.
			template <typename T, typename Handler>
				void async_write(const T& t, Handler handler)
				{
					// Serialize the data first so we know how large it is.
					std::ostringstream archive_stream;
					boost::archive::text_oarchive archive(archive_stream);
					archive << t;
					outbound_data_ = archive_stream.str();

					// Format the header.
					std::ostringstream header_stream;
					header_stream << std::setw(header_length)
						<< std::hex << outbound_data_.size();
					if (!header_stream || header_stream.str().size() != header_length)
					{
						// Something went wrong, inform the caller.
						boost::system::error_code error(boost::asio::error::invalid_argument);
						socket_.get_io_service().post(boost::bind(handler, error));
						return;
					}
					outbound_header_ = header_stream.str();

					// Write the serialized data to the socket. We use "gather-write" to send
					// both the header and the data in a single write operation.
					std::vector<boost::asio::const_buffer> buffers;
					buffers.push_back(boost::asio::buffer(outbound_header_));
					buffers.push_back(boost::asio::buffer(outbound_data_));
					boost::asio::async_write(socket_, buffers, handler);
				}

				void sync_write(std::istream& iss)
				{
					// Serialize the data first so we know how large it is.
					// std::ostringstream archive_stream;
					// boost::archive::text_oarchive archive(archive_stream);
					std::istreambuf_iterator<char> eos;
					std::string outbound_data(std::istreambuf_iterator<char>(iss), eos);

					std::cout << "write " << outbound_data.size() << " " << outbound_data << std::endl;

					// Format the header.
					std::ostringstream header_stream;
					header_stream << std::setw(header_length)
						<< std::hex << outbound_data.size();
					if (!header_stream || header_stream.str().size() != header_length)
					{
						// Something went wrong, inform the caller.
						boost::system::error_code error(boost::asio::error::invalid_argument);
						std::cerr << "error in header" << std::endl;
						return;
					}
					outbound_header_ = header_stream.str();

					// Write the serialized data to the socket. We use "gather-write" to send
					// both the header and the data in a single write operation.
					std::vector<boost::asio::const_buffer> buffers;
					buffers.push_back(boost::asio::buffer(outbound_header_));
					buffers.push_back(boost::asio::buffer(outbound_data));
					boost::asio::write(socket_, buffers);
				}

			/// Asynchronously read a data structure from the socket.
			template <typename T, typename Handler>
				void async_read(T& t, Handler handler)
				{
					// Issue a read operation to read exactly the number of bytes in a header.
					void (connection::*f)(
							const boost::system::error_code&,
							T&, boost::tuple<Handler>)
						= &connection::handle_read_header<T, Handler>;
					boost::asio::async_read(socket_, boost::asio::buffer(inbound_header_),
							boost::bind(f,
								this, boost::asio::placeholders::error, boost::ref(t),
								boost::make_tuple(handler)));
				}

			/// synchronously read a data structure from the socket.
			// void sync_read(pop::bufin& oa)
			void sync_read(std::ostream& ost)
			{
				// Issue a read operation to read exactly the number of bytes in a header.
				boost::asio::read(socket_, boost::asio::buffer(inbound_header_));
				// Determine the length of the serialized data.
				std::istringstream is(std::string(inbound_header_, header_length));
				std::size_t inbound_data_size = 0;
				if (!(is >> std::hex >> inbound_data_size))
				{
					// Header doesn't seem to be valid. Inform the caller.
					boost::system::error_code error(boost::asio::error::invalid_argument);
					cerr << "read error"<<endl;
					return;
				}

				// Start an asynchronous call to receive the data.
				inbound_data_.resize(inbound_data_size);
				boost::asio::read(socket_, boost::asio::buffer(inbound_data_));

				// Extract the data structure from the data just received.
				try
				{
					std::cout<<"read length " << inbound_data_.size() << std::endl;
					std::string archive_data(&inbound_data_[0], inbound_data_.size());
					// std::istringstream archive_stream(archive_data);
					// boost::archive::text_iarchive archive(archive_stream);
					// archive >> ia;
					ost << archive_data;
				}
				catch (std::exception& e)
				{
					// Unable to decode data.
					boost::system::error_code error(boost::asio::error::invalid_argument);
					cerr << "error A" <<endl;
					return;
				}

				// Inform caller that data has been received ok.
				// boost::get<0>(handler)(e);
				cout << "data read" <<endl;
			}

			/// Handle a completed read of a message header. The handler is passed using
			/// a tuple since boost::bind seems to have trouble binding a function object
			/// created using boost::bind as a parameter.
			template <typename T, typename Handler>
				void handle_read_header(const boost::system::error_code& e,
						T& t, boost::tuple<Handler> handler)
				{
					if (e)
					{
						boost::get<0>(handler)(e);
					}
					else
					{
						// Determine the length of the serialized data.
						std::istringstream is(std::string(inbound_header_, header_length));
						std::size_t inbound_data_size = 0;
						if (!(is >> std::hex >> inbound_data_size))
						{
							// Header doesn't seem to be valid. Inform the caller.
							boost::system::error_code error(boost::asio::error::invalid_argument);
							boost::get<0>(handler)(error);
							return;
						}

						// Start an asynchronous call to receive the data.
						inbound_data_.resize(inbound_data_size);
						void (connection::*f)(
								const boost::system::error_code&,
								T&, boost::tuple<Handler>)
							= &connection::handle_read_data<T, Handler>;
						boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),
								boost::bind(f, this,
									boost::asio::placeholders::error, boost::ref(t), handler));
					}
				}

			/// Handle a completed read of message data.
			template <typename T, typename Handler>
				void handle_read_data(const boost::system::error_code& e,
						T& t, boost::tuple<Handler> handler)
				{
					if (e)
					{
						boost::get<0>(handler)(e);
					}
					else
					{
						// Extract the data structure from the data just received.
						try
						{
							std::string archive_data(&inbound_data_[0], inbound_data_.size());
							std::istringstream archive_stream(archive_data);
							boost::archive::text_iarchive archive(archive_stream);
							archive >> t;
						}
						catch (std::exception& e)
						{
							// Unable to decode data.
							boost::system::error_code error(boost::asio::error::invalid_argument);
							boost::get<0>(handler)(error);
							return;
						}

						// Inform caller that data has been received ok.
						boost::get<0>(handler)(e);
					}
				}


		private:

			/// The underlying socket.
			boost::asio::ip::tcp::socket socket_;

			/// The size of a fixed length header.
			enum { header_length = 8 };

			/// Holds an outbound header.
			std::string outbound_header_;

			/// Holds the outbound data.
			std::string outbound_data_;

			/// Holds an inbound header.
			char inbound_header_[header_length];

			/// Holds the inbound data.
			std::vector<char> inbound_data_;
	};

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
				std::cout<<__LINE__<<std::endl;
				boost::asio::ip::tcp::resolver resolver(io_service_);
				std::cout<<__LINE__<<std::endl;
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
				std::cout<<__LINE__<<std::endl;
				boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
				std::cout<<__LINE__<<std::endl;

				// Start an asynchronous connect operation.
				std::cout<<"async connect"<<std::endl;
				connection_.socket().async_connect(endpoint, boost::bind(&client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator));
				io_service_.run();
			}

			inline void run(){io_service_.run();}
			inline connection& connec(){return connection_;}


		private:
			/// Handle completion of a connect operation.
			void handle_connect(const boost::system::error_code& e, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
			{
				std::cout<<"handle connect"<<std::endl;
				if (!e)
				{
					std::cout<<"connected"<<std::endl;
				}
				else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
				{
					// Try the next endpoint.
					connection_.socket().close();
					boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
					connection_.socket().async_connect(endpoint,
							boost::bind(&client::handle_connect, this,
								boost::asio::placeholders::error, ++endpoint_iterator));
				}
				else
				{
					// An error occurred. Log it and return. Since we are not starting a new
					// operation the io_service will run out of work to do and the client will
					// exit.
					std::cerr << e.message() << std::endl;
				}
			}

			boost::asio::io_service io_service_;
			connection connection_;
			const boost::asio::ip::tcp::resolver::query& query_;
	};

	typedef boost::shared_ptr<connection> connection_ptr;

} // namespace

#endif
