#ifndef POPLITE_INTERFACE_H
#define POPLITE_INTERFACE_H


#include "com/serialize.hpp"
#include "com/interface_combox.hpp"
#include "alloc/local.hpp"

namespace pop{

class interface
{
	public:
		interface(const pop::allocator& _allocator) :
			combox_()
	{

		_allocator.allocate("remote_main", combox_.endpoint());
		// Handle connection
		combox_.run();
	}

		// interface(boost::asio::ip::tcp::endpoint& _endpoint) :


		template<typename ...Args> void call_sync(int _method_id, Args ...args)
		{
			try
			{
				std::tuple<Args...> tup(std::forward_as_tuple(args...));
				std::stringstream oss1;
				bufout oa1(oss1);
				oa1 << _method_id;
				combox_.connec().sync_write(oss1);

				std::stringstream oss2;
				bufout oa2(oss2);
				oa2 << tup;
				combox_.connec().sync_write(oss2);

				LOG(debug) << "send to broker";

				std::stringstream iss;
				combox_.connec().sync_read(iss);
				bufin ia(iss);
				ia >> tup;
				LOG(debug) << "received answer from broker";

				std::stringstream iss2;
				combox_.connec().sync_read(iss2);
				bufin ia2(iss2);
				std::string ack;
				ia2 >> ack;
				LOG(debug) << "received ack=" << ack;
				if(ack != "ACK")
					throw std::runtime_error("did not receive ack");
				if(_method_id == -1)
					combox_.connec().socket().close();

			}
			catch(std::exception& e)
			{
				LOG(error) << "exception in call_sync: " << e.what();
			}
		}
	private:
		pop::interface_combox combox_;
};



}

#endif
