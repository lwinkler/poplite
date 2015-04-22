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
			connection_(query)
		{
		}


		template<typename ...Args> void call_sync(int x_method_id, std::tuple<Args...>& tup)
		{
			try
			{
				std::stringstream oss;
				bufout oa(oss);
				oa << x_method_id;
				oa << tup;

				connection_.sync_write(oss);

				std::stringstream iss;
				bufin ia(iss);
				connection_.sync_read(iss);

				ia >> tup;
			}
			catch(std::exception& e)
			{
				std::cerr << "exception in call_sync: " << e.what() << std::endl;
			}
		}
	private:
		boost::asio::ip::tcp::resolver::query query_;
		pop::connection connection_;
		boost::asio::ip::tcp::endpoint endpoint_;
		// bufin&  ia;
		// bufout& oa;
};



}

#endif
