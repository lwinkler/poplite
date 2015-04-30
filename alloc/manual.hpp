#ifndef POP_ALLOC_MANUAL_H
#define POP_ALLOC_MANUAL_H


#include "alloc/alloc.hpp"

namespace pop{

class manual_allocator : public allocator
{
	public:
	manual_allocator(){}
	void allocate(const std::string& _obj_name, const boost::asio::ip::tcp::endpoint& _endpoint) const
	{
		std::stringstream ss;
		ss << "./" << _obj_name << " " << _endpoint.address() << " " << _endpoint.port();
		LOG(info) << "Start object " << _obj_name << " with command :";
		LOG(info) << ss.str();
	}
};
}

#endif
