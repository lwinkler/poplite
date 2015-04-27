#ifndef POPLITE_BROKER_H
#define POPLITE_BROKER_H


#include <iostream>
#include "class/system.hpp"
#include "com/serialize.hpp"


namespace pop
{
namespace remote
{
	// typedef vector<parallel_method> parallel_methods;
	template<class ParClass>
		using parallel_method = std::function<void(pop::bufin&, pop::bufout&, ParClass&)>;

template<class ParClass>
class broker
{
	public:
		broker(const std::vector<parallel_method<ParClass>>& _methods) : 
			obj_(), 
			methods_(_methods)
		{}
		inline void remote_call(int nb, bufin& ia, bufout& oa) {(methods_.at(nb))(ia, oa, obj_);}

		/// A simple call to a method 
		template<typename ...Args> static void call_simple(pop::bufin& ia, pop::bufout& oa, ParClass& obj, void (ParClass::*xp_meth)(std::tuple<Args...>&))
		{
			std::tuple<Args...> tup;
			ia >> tup;
			(obj.*xp_meth)(tup);
			oa << tup;
		}

	private:

		const std::vector<parallel_method<ParClass>> methods_;
		ParClass obj_;
};

}} // namespaces

#endif
