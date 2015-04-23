#ifndef POPLITE_SYSTEM_H
#define POPLITE_SYSTEM_H

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

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
