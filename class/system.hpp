#ifndef POPLITE_SYSTEM_H
#define POPLITE_SYSTEM_H

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
// #include <boost/archive/binary_oarchive.hpp>
// #include <boost/archive/binary_iarchive.hpp>

namespace pop
{
#if 1
typedef boost::archive::text_iarchive bufin;
typedef boost::archive::text_oarchive bufout;
#else
typedef boost::archive::binary_iarchive bufin;
typedef boost::archive::binary_oarchive bufout;
#endif

}


#endif
