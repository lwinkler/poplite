#ifndef POP_ALLOC_H
#define POP_ALLOC_H


#include <boost/asio/ip/tcp.hpp>

#define MAX_STR 512

namespace pop{

class allocator
{
	public:
	virtual void allocate(const std::string& _obj_name, const boost::asio::ip::tcp::endpoint& _endpoint) const = 0;
};
}

#endif
