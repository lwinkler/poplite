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

namespace pop {

	class accesspoint
	{
		public:
			accesspoint() : host_name_(""), port_(0) {}

			accesspoint(const boost::asio::ip::tcp::endpoint& _contact_endpoint) : 
				host_name_(boost::asio::ip::host_name()), // TODO: Try with address().to_string()
				port_(_contact_endpoint.port())
		{
			/*
			   while(_iterator != boost::asio::ip::tcp::resolver::iterator())
			   {
			//resol.resolve(*_iterator);
			LOG(debug) << "address:" << _iterator->endpoint().address().to_string();
			++_iterator;
			}
			 */
		}
			std::string host_name_;
			int port_;


			boost::asio::ip::tcp::resolver::query create_query() const
			{
				boost::asio::ip::tcp::resolver::query q(host_name_, std::to_string(port_));
				return q;
			}

		private:
			friend class boost::serialization::access;
			template<class Archive>void serialize(Archive & ar, const unsigned int version)
			{
				ar & host_name_;
				ar & port_;
			}

	};

} // namespace

#endif
