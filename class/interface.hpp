#ifndef POPLITE_INTERFACE_H
#define POPLITE_INTERFACE_H


#include "com/serialize.hpp"
#include "com/interface_combox.hpp"

namespace pop{

class interface
{
	public:
		interface(boost::asio::ip::tcp::endpoint& _endpoint) :
			client_(_endpoint)
		{}


		template<typename ...Args> void call_sync(int x_method_id, std::tuple<Args...>& tup)
		{
			try
			{
				std::stringstream oss1;
				bufout oa1(oss1);
				oa1 << x_method_id;
				client_.connec().sync_write(oss1);

				std::stringstream oss2;
				bufout oa2(oss2);
				oa2 << tup;
				client_.connec().sync_write(oss2);

				LOG(debug) << "send to broker";

				std::stringstream iss;
				client_.connec().sync_read(iss);
				bufin ia(iss);
				ia >> tup;
				LOG(debug) << "received answer from broker";

				std::stringstream iss2;
				client_.connec().sync_read(iss2);
				bufin ia2(iss2);
				std::string ack;
				ia2 >> ack;
				LOG(debug) << "received ack=" << ack;
				if(ack != "ACK")
					throw std::runtime_error("did not receive ack");
			}
			catch(std::exception& e)
			{
				LOG(error) << "exception in call_sync: " << e.what();
			}
		}
	private:
		pop::interface_combox client_;
		boost::asio::ip::tcp::endpoint endpoint_;
		// bufin&  ia;
		// bufout& oa;
};



}

#endif
