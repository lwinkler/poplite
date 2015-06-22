//
// broker.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef POPLITE_BROKER_H
#define POPLITE_BROKER_H


#include <iostream>
#include "class/system.hpp"
#include "class/apply_tuple.hpp"
#include "class/apply_tuple_constr.hpp"
#include "com/serialize.hpp"


namespace pop
{
	namespace remote
	{
		template<class ParClass> using parallel_method = std::function<void(bufin&, bufout&, ParClass*&)>;

		/// A broker is the (remote) part that contains the instanciation of the parallel object
		template<class ParClass> class broker : private boost::noncopyable
		{
			public:
				broker() : p_obj_(nullptr){}

				~broker()
				{
					if(p_obj_)
						delete p_obj_;
				}

				inline void remote_call(int _nb, bufin& _ia, bufout& _oa) {(methods_.at(_nb))(_ia, _oa, p_obj_);}
			private:

				/// A call to constructor
				template<typename ...Args> static ParClass* __constr(Args... args){return new ParClass(args...);}
				template<typename ...Args> static void call_constr(bufin& _ia, bufout& _oa, ParClass*& _p_obj)
				{
					if(_p_obj)
						throw std::runtime_error("Constructor has been called twice");
					std::tuple<Args...> tup;
					_ia >> tup;
					_p_obj = applyTupleConstr(__constr<Args...>, tup);
					_oa << tup;
				}

				/// A simple concurrent call to a method 
				template<typename R, typename ...Args> static void conc(bufin& ia, bufout& oa, ParClass*& _p_obj, R (ParClass::*_p_meth)(Args...)) // TODO: Args by ref ?
				{
					if(!_p_obj)
						throw std::runtime_error("Constructor has not been called");
					std::tuple<typename std::decay<Args>::type...> tup;
					ia >> tup;
					apply_tuple(_p_obj, _p_meth, tup, oa);
				}


			private:
				// The list of parallel methods: will be generated by the parser and added at link
				static const std::vector<remote::parallel_method<ParClass>> methods_;
				ParClass* p_obj_;
		};

	}
} // namespaces

#endif
