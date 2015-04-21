#ifndef POPLITE_BROKER_H
#define POPLITE_BROKER_H


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
// #include "tcp_connection.hpp" // Must come before boost/serialization headers.
#include "com/serialize.hpp"


namespace pop
{
namespace remote
{
	using namespace std;
	using namespace pop;

	// typedef vector<parallel_method> parallel_methods;
	template<class ParClass>
		using parallel_method = std::function<void(pop::bufin&, pop::bufout&, ParClass&)>;

template<class ParClass>
class broker
{
	public:
		// typedef void (broker::*pt_method)(pop::bufin& ia, pop::bufout& oa);
		broker(const vector<parallel_method<ParClass>>& xr_methods) : 
			obj(), 
			methods(xr_methods)
		{
		}

		/// A simple call to a method 
		template<typename ...Args> static void call_simple(pop::bufin& ia, pop::bufout& oa, ParClass& obj, void (ParClass::*xp_meth)(std::tuple<Args...>&))
		{
			std::tuple<Args...> tup;
			ia >> tup;
			(obj.*xp_meth)(tup);
			oa << tup;
		}


	private:
		enum { header_length = 8 };
		inline void remote_call(int nb, bufin& ia, bufout& oa) {(methods.at(nb))(ia, oa, obj);}
		vector<parallel_method<ParClass>> methods;
		ParClass obj;
};

}} // namespaces

#endif
