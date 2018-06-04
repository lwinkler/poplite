//
// accesspoint.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef POP_ACCESSPOINT_HPP
#define POP_ACCESSPOINT_HPP

#include <boost/asio.hpp>

#include "com/serialize.hpp"

/// A point of access (via communication interface) that is serializable
namespace pop {

class accesspoint {
public:
	accesspoint() noexcept : host_name(""), port(0) {}

	/*accesspoint(const boost::asio::ip::tcp::endpoint& _contact_endpoint) :
		host_name(pop::system::instance().host_name()),
		port(_contact_endpoint.port())
	{}*/
	accesspoint(const std::string& _host_name, int _port) :
		host_name(_host_name),
		port(_port)
	{}

	bool operator == (const accesspoint& _ap) {
		return host_name == _ap.host_name && port == _ap.port;
	}
	bool operator != (const accesspoint& _ap) {
		return host_name != _ap.host_name || port != _ap.port;
	}

	std::string host_name;
	int port;


	boost::asio::ip::tcp::resolver::query create_query() const {
		boost::asio::ip::tcp::resolver::query q(host_name, std::to_string(port));
		return q;
	}

private:
	friend class boost::serialization::access;
	template<class Archive>void serialize(Archive & ar, const unsigned int /*version*/) {
		ar & host_name;
		ar & port;
	}

};

} // namespace

#endif
