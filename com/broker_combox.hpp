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
#include "com/exception.hpp"
#include <thread>


namespace pop {


/// Remove server for each parallel object
template<class Brok>class broker_combox {
public:
	/// Constructor opens the acceptor and starts waiting for the first incoming
	/// connection.
	broker_combox(Brok& _brok, const boost::asio::ip::tcp::resolver::query & _query) : broker_combox(_brok) {
		boost::asio::ip::tcp::resolver resolver(io_service_);
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(_query);
		boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

		// Start an asynchronous connect operation for the connecting interface
		LOG(debug) <<"async connect " << accesspoint_.host_name << " " << accesspoint_.port;
		connection_ptr new_conn(new connection(io_service_));
		new_conn->socket().async_connect(endpoint, boost::bind(&broker_combox::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator, new_conn));
	}

	/// Simplified constructor for local objects. Does not connect to interface
	broker_combox(Brok& _brok) :
		brok_(_brok),
		service_acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0 /*port*/)),
		accesspoint_(pop::system::instance().host_name(), service_acceptor_.local_endpoint().port()) {
		// Accept connection from other interfaces
		connection_ptr service_connection(new connection(io_service_));
		service_acceptor_.async_accept(service_connection->socket(), boost::bind(&broker_combox::handle_accept_service, this, boost::asio::placeholders::error, service_connection));
	}

	/*
	static void run_handler_in_new_thread(boost::asio::io_service& _service, size_t& _nb_extra_threads) {
		std::thread th([&_service, &_nb_extra_threads]{
			_nb_extra_threads++;
			LOG(debug) << "extra threads " << _nb_extra_threads;
			_service.run_one();
			_nb_extra_threads--;
		});
		th.detach();
	}
	*/

	/// Run io server
	inline void run() {
		std::vector<std::thread> workers;
		// note: so far we can only set up a fixed number of threads
		//       this determines the max number of simultaneous calls to the broker
		for (int i = 0; i < 10; i++) { // TODO: const in pop::system ? https://www.gamedev.net/blogs/entry/2249317-a-guide-to-getting-started-with-boostasio/
			workers.push_back(std::thread([&]() {
				io_service_.run();
			}));
		}
		std::for_each(workers.begin(), workers.end(), [](std::thread &t) {
			t.join();
		});
	}

	inline void stop() {
		io_service_.stop();
	}

	inline const pop::accesspoint& contact() const {
		// return service_acceptor_.local_endpoint();
		return accesspoint_;
	}

private:
	/// Handle completion of a connect operation.
	void handle_connect(const boost::system::error_code& _e, boost::asio::ip::tcp::resolver::iterator _endpoint_iterator, connection_ptr _conn) {
		if (!_e) {
			LOG(debug) << "send contact from broker"; // << accesspoint_;
			_conn->sync_write(accesspoint_);

			LOG(debug)<<"connected";
		} else if (_endpoint_iterator != boost::asio::ip::tcp::resolver::iterator()) {
			// Try the next endpoint.
			LOG(debug) << "try next endpoint";
			_conn->socket().close();
			boost::asio::ip::tcp::endpoint endpoint = *_endpoint_iterator;
			_conn->socket().async_connect(endpoint, boost::bind(&broker_combox::handle_connect, this, boost::asio::placeholders::error, ++_endpoint_iterator, _conn));
		} else {
			throw std::runtime_error("connection failed");
		}
		LOG(debug) << "broker starts listening";
		_conn->async_read(boost::bind(&broker_combox::handle_read, this, boost::asio::placeholders::error, _conn));
	}


	void handle_read(const boost::system::error_code& _e, connection_ptr _conn) {
		if(_e) {
			throw std::runtime_error("handle_read: " + _e.message());
		}
		// Receive an incomming remote method invocation
		// Successfully accepted a new connection. Call method by id
		bufin ia(_conn->input_stream());
		method_id_t meth_id = method_ids::UNKNOWN;
		ia >> meth_id;

		LOG(debug) << "method id " << meth_id;

		bool is_async = false;
		ia >> is_async;
		LOG(debug) << "async " << is_async;

		static const pop::exception ack;
		std::stringstream oss;
		bufout oa(oss);
		LOG(debug) << "call remote method " << meth_id;

		if(meth_id == method_ids::DISCONNECT) {
			LOG(debug) << "disconnect iface and broker";
			_conn->socket().close();
			return;
		}
		if(meth_id == method_ids::DESTROY) {
			LOG(debug) << "received end signal";
			_conn->socket().close();
			stop();
			return;
		} else if(is_async) {
			// if the call is asynchronous, we send the ack directly
			oa << ack;
			_conn->sync_write_ss(oss);
			LOG(debug) << "sent ack";
			try {
				brok_.remote_call(meth_id, ia, oa);
			} catch(std::exception& exc) {
				LOG(error) << "Exception caught in async method: " << exc.what();
				// exc.text = "Exception caught in async method: ";
				// exc.text += exc.what();
			} catch(...) {
				LOG(error) << "Unknown exception caught in async method";
				// exc.text = "Exception caught in async method";
			}
			LOG(debug) << "finish calling remote method " << meth_id;
		} else {
			pop::exception remote_exc;
			try {
				brok_.remote_call(meth_id, ia, oa);
			} catch(std::exception& exc) {
				LOG(warning) << "Broker: Exception caught in sync method: " << exc.what();
				// remote_exc.text = "Exception caught in sync method: ";
				remote_exc.text = exc.what();
			} catch(...) {
				LOG(warning) << "Broker: Unknown exception caught in sync method";
				remote_exc.text = "Unknown exception";
			}

			LOG(debug) << "finish calling remote method " << meth_id;

			oa << remote_exc;
			_conn->sync_write_ss(oss);
			LOG(debug) << "sent ack";
		}

		LOG(debug) << "broker re-starts listening";
		_conn->async_read(boost::bind(&broker_combox::handle_read, this, boost::asio::placeholders::error, _conn));
	}

	/// Handle contact by a new interface
	void handle_accept_service(const boost::system::error_code& _e, connection_ptr _service_connection) {
		if(_e) {
			throw std::runtime_error(_e.message());
		}
		_service_connection->sync_read();
		bufin ia(_service_connection->input_stream());
		service_type stype = service_type::UNKNOWN;
		ia >> stype;
		LOG(debug) << "Interface combox contacted";
		switch(stype) {
			case service_type::CLOSE_SERVICE_CONNECTION:
				// Closing the service:
				LOG(debug) << "Closing service on broker";
				return;
			case service_type::OPEN_SERVICE_CONNECTION: {
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
				throw std::runtime_error("Unknown service_type " + std::to_string(static_cast<int>(stype)));
		}

		// Recreate a connection for contact
		connection_ptr new_conn2(new connection(io_service_));
		service_acceptor_.async_accept(new_conn2->socket(), boost::bind(&broker_combox::handle_accept_service, this, boost::asio::placeholders::error, new_conn2));
	}

private:
	/// The data to be sent to each client.
	Brok& brok_;
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::acceptor service_acceptor_;
	pop::accesspoint accesspoint_;

	// size_t nb_extra_threads_ = 0;
};

} // namespace

#endif
