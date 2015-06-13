#ifndef POP_ALLOC_H
#define POP_ALLOC_H


#include <boost/asio/ip/tcp.hpp>

#define MAX_STR 512

namespace pop{

/// Parent class for all allocators
/// allocators can launch an object on a local or remote platform

class allocator
{
	public:
	/// Allocate an object at endpoint
	virtual void allocate(const std::string& _obj_name, const boost::asio::ip::tcp::endpoint& _endpoint) const = 0;
};
}

#endif
