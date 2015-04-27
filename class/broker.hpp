#ifndef POPLITE_BROKER_H
#define POPLITE_BROKER_H


#include <iostream>
#include "class/system.hpp"
#include "com/serialize.hpp"


namespace pop
{
namespace remote
{
	template<class ParClass>
		using parallel_method = std::function<void(pop::bufin&, pop::bufout&, ParClass*&)>;

template<class ParClass>
class broker
{
	public:
		broker(const std::vector<parallel_method<ParClass>>& _methods) : 
			p_obj_(nullptr), 
			methods_(_methods)
		{}

		~broker()
		{
			if(p_obj_)
				delete p_obj_;
		}

		inline void remote_call(int _nb, bufin& _ia, bufout& _oa) {(methods_.at(_nb))(_ia, _oa, p_obj_);}

		/// A call to constructor
		template<typename ...Args> static void call_constr(pop::bufin& _ia, pop::bufout& _oa, ParClass*& _p_obj, ParClass* (*_p_meth)(std::tuple<Args...>&))
		{
			if(_p_obj)
				throw std::runtime_error("Constructor has been called twice");
			std::tuple<Args...> tup;
			_ia >> tup;
			_p_obj = (*_p_meth)(tup);
			_oa << tup;
		}

		/// A simple call to a method 
		template<typename ...Args> static void call_simple(pop::bufin& ia, pop::bufout& oa, ParClass*& _p_obj, void (ParClass::*_p_meth)(std::tuple<Args...>&))
		{
			if(!_p_obj)
				throw std::runtime_error("Constructor has not been called");
			std::tuple<Args...> tup;
			ia >> tup;
			(_p_obj->*_p_meth)(tup);
			oa << tup;
		}

	private:

		const std::vector<parallel_method<ParClass>> methods_;
		ParClass* p_obj_;
};

}} // namespaces

#endif
