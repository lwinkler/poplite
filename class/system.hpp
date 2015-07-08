//
// system.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POPLITE_SYSTEM_H
#define POPLITE_SYSTEM_H


#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "parser/defs.hpp"

// Lines for boost log
#define BOOST_LOG_DYN_LINK
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#define LOG BOOST_LOG_TRIVIAL

namespace pop
{
#ifndef POP_BINARY_SERIALIZATION
// use text serialization
typedef boost::archive::text_iarchive bufin;
typedef boost::archive::text_oarchive bufout;
#else
// use binary serialization
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
typedef boost::archive::binary_iarchive bufin;
typedef boost::archive::binary_oarchive bufout;
#endif


}


#endif
