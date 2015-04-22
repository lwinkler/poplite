#ifndef POPLITE_INTERFACE_H
#define POPLITE_INTERFACE_H


#include "com/serialize.hpp"
#include "com/connection.hpp"

namespace pop{

class interface
{
	public:
		interface(const boost::asio::ip::tcp::resolver::query & query) :
			query_(query),
			client_(query_)
		{
				std::cout<<__LINE__<<std::endl;
		}


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

				std::cout << "send to broker" << std::endl;

				std::stringstream iss;
				client_.connec().sync_read(iss);
				cout << __LINE__ << endl;
				bufin ia(iss);
				cout << __LINE__ << endl;
				std::cout << "received from broker" << std::endl;

				ia >> tup;
			}
			catch(std::exception& e)
			{
				std::cerr << "exception in call_sync: " << e.what() << std::endl;
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
