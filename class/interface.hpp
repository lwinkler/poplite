#ifndef POPLITE_INTERFACE_H
#define POPLITE_INTERFACE_H


#include "com/serialize.hpp"
#include "com/client.hpp"

namespace pop{

class interface
{
	public:
		interface(const boost::asio::ip::tcp::resolver::query & query) :
			query_(query),
			client_(query_)
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
			}
			catch(std::exception& e)
			{
				LOG(error) << "exception in call_sync: " << e.what();
			}
		}
	private:
		boost::asio::ip::tcp::resolver::query query_;
		pop::client client_;
		boost::asio::ip::tcp::endpoint endpoint_;
		// bufin&  ia;
		// bufout& oa;
};



}

#endif
