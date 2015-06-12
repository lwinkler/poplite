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

#include "class/util.hpp"
#include "com/serialize.hpp"

namespace pop {

	class connection
	{
		public:
			/// Constructor
			connection(boost::asio::io_service& io_service) : socket_(io_service) {}

			/// Get the underlying socket. Used for making a connection or for accepting
			/// an incoming connection.
			inline boost::asio::ip::tcp::socket& socket(){return socket_;}

			/// Return ref to the input stream
			inline std::istream& input_stream(){return iss_;}


			/// Asynchronously write a data structure to the socket.
			template <typename T, typename Handler> void async_write(const T& _obj, Handler _handler)
			{
					std::stringstream ss;
					bufout ia(ss);
					ia << _obj;
					async_write_ss(ss, _handler);
			}

			/// Asynchronously write a stringstream to the socket.
			template <typename Handler> void async_write_ss(std::istream& _iss, Handler _handler)
			{
				std::istreambuf_iterator<char> eos;
				std::string outbound_data(std::istreambuf_iterator<char>(_iss), eos);

				// Format the header.
				std::ostringstream header_stream;
				header_stream << std::setw(header_length) << std::hex << outbound_data.size();
				if (!header_stream || header_stream.str().size() != header_length)
				{
					// Something went wrong, inform the caller.
					boost::system::error_code error(boost::asio::error::invalid_argument);
					socket_.get_io_service().post(boost::bind(_handler, error));
					return;
				}
				outbound_header_ = header_stream.str();

				// Write the serialized data to the socket. We use "gather-write" to send
				// both the header and the data in a single write operation.
				std::vector<boost::asio::const_buffer> buffers;
				buffers.push_back(boost::asio::buffer(outbound_header_));
				buffers.push_back(boost::asio::buffer(outbound_data));
				boost::asio::async_write(socket_, buffers, _handler);
			}

			// Synchronous write an object to the socket
			template <typename T> void sync_write(const T& _obj)
			{
					std::stringstream ss;
					bufout ia(ss);
					ia << _obj;
					sync_write_ss(ss);
			}

			// Synchronous write to the socket
			void sync_write_ss(std::istream& _iss)
			{
				std::istreambuf_iterator<char> eos;
				std::string outbound_data(std::istreambuf_iterator<char>(_iss), eos);

				LOG(debug) << "sync write " << outbound_data.size() << " " << outbound_data;

				// Format the header.
				std::ostringstream header_stream;
				header_stream << std::setw(header_length) << std::hex << outbound_data.size();
				if (!header_stream || header_stream.str().size() != header_length)
				{
					// Something went wrong, inform the caller.
					boost::system::error_code error(boost::asio::error::invalid_argument);
					throw std::runtime_error("error header in sync_write");
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
			template <typename Handler>void async_read(Handler _handler)
			{
				// Reset the stringstream
				iss_.str("");
				iss_.clear();

				// Issue a read operation to read exactly the number of bytes in a header.
				auto f = &connection::handle_read_header<Handler>;
				boost::asio::async_read(
					socket_, 
					boost::asio::buffer(inbound_header_),
					boost::bind(f, this, boost::asio::placeholders::error, boost::ref(iss_), boost::make_tuple(_handler))
				);
			}

			/// Synchronously read a data structure from the socket.
			void sync_read()
			{
				// Reset the stringstream
				iss_.str("");
				iss_.clear();

				// Issue a read operation to read exactly the number of bytes in a header.
				boost::asio::read(socket_, boost::asio::buffer(inbound_header_));
				// Determine the length of the serialized data.
				std::istringstream is(std::string(inbound_header_, header_length));
				std::size_t inbound_data_size = 0;
				if (!(is >> std::hex >> inbound_data_size))
				{
					// Header doesn't seem to be valid. Inform the caller.
					boost::system::error_code error(boost::asio::error::invalid_argument);
					LOG(error) << "read error";
					return;
				}

				// Start an asynchronous call to receive the data.
				inbound_data_.resize(inbound_data_size);
				boost::asio::read(socket_, boost::asio::buffer(inbound_data_));

				// Extract the data structure from the data just received.
				try
				{
					std::string archive_data(&inbound_data_[0], inbound_data_.size());
					iss_ << archive_data;
				}
				catch (std::exception& e)
				{
					// Unable to decode data.
					throw std::runtime_error("error in sync_read: " + std::string(e.what()));
				}

				LOG(debug) << "data read: " << inbound_data_[0];
			}

			/// Handle a completed read of a message header. The handler is passed using
			/// a tuple since boost::bind seems to have trouble binding a function object
			/// created using boost::bind as a parameter.
			template <typename Handler>
				void handle_read_header(const boost::system::error_code& _e, std::ostream& _oss, boost::tuple<Handler> _handler)
				{
					if (_e)
					{
						boost::get<0>(_handler)(_e);
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
							boost::get<0>(_handler)(error);
							return;
						}

						// Start an asynchronous call to receive the data.
						inbound_data_.resize(inbound_data_size);
						void (connection::*f)(const boost::system::error_code&, std::ostream&, boost::tuple<Handler>)
							= &connection::handle_read_data<Handler>;
						boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),
							boost::bind(f, this, boost::asio::placeholders::error, boost::ref(_oss), _handler));
					}
				}

			/// Handle a completed read of message data.
			template <typename Handler>
				void handle_read_data(const boost::system::error_code& e, std::ostream& _oss, boost::tuple<Handler> _handler)
				{
					if (e)
					{
						boost::get<0>(_handler)(e);
					}
					else
					{
						// Extract the data structure from the data just received.
						try
						{
							copy(inbound_data_.begin(), inbound_data_.end(), std::ostream_iterator<unsigned char>(_oss));
							/*
							std::string archive_data(&inbound_data_[0], inbound_data_.size());
							std::istringstream archive_stream(archive_data);
							boost::archive::text_iarchive archive(archive_stream);
							archive >> t;
							*/
						}
						catch (std::exception& e)
						{
							// Unable to decode data.
							boost::system::error_code error(boost::asio::error::invalid_argument);
							boost::get<0>(_handler)(error);
							return;
						}

						// Inform caller that data has been received ok.
						boost::get<0>(_handler)(e);
					}
				}

		private:
			// A stream that we use to conveniently read the input
			std::stringstream iss_;

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

	typedef boost::shared_ptr<connection> connection_ptr;

} // namespace

#endif
