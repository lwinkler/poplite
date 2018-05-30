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
#include "class/apply_tuple_static.hpp"
#include "class/apply_tuple_const.hpp"
#include "com/serialize.hpp"
#include "class/interface.hpp"

namespace pop
{
namespace remote
{
template<class ParClass> using parallel_method = std::function<void(bufin&, bufout&, std::unique_ptr<ParClass>&)>;

/// A utility container to store and serialize an interface
template<class T> struct iface_container final {
	// overload cast to T
	operator T&() {
		return *p_iface_;
	}
	operator const T&() {
		return *p_iface_;
	}
	~iface_container() {
		delete p_iface_;
	}


private:
	T* p_iface_ = nullptr;
	friend class boost::serialization::access;
	template<class Archive> void save(Archive & _ar, const unsigned int _version) const
	{
		// invoke serialization of the base class
		LOG(debug) << "Save interface container at " << p_iface_->contact().host_name;
		assert(p_iface_ != nullptr);
		_ar << p_iface_->contact();
	}

	template<class Archive> void load(Archive & _ar, const unsigned int _version)
	{
		pop::accesspoint ap;
		_ar >> ap;
		assert(p_iface_ == nullptr);
		p_iface_ = new T(ap);
	}

	template<class Archive> void serialize(Archive & _ar, const unsigned int _version) {
		boost::serialization::split_member(_ar, *this, _version);
	}

};

/// A utility to decay incoming arguments
template<class T> struct pop_decay
{
	typedef typename std::decay<T>::type type;
};

template<class T> struct pop_decay<T&>
{
	/// typedef pop::accesspoint type;
	typedef typename std::conditional<std::is_base_of<pop::interface, typename std::remove_reference<T>::type>::value, iface_container<T>, typename std::decay<T>::type>::type type;
	// typedef typename std::decay<T>::type type;
};

template<class Archive, typename... Args>
void serialize_out(Archive & ar, std::tuple<typename pop_decay<Args>::type...> & t1)
{
	SerializeOut<sizeof...(Args)>::template serialize_out<Archive, std::tuple<Args&...>, std::tuple<typename pop_decay<Args>::type...> >(ar, t1);
}

// create a method pointer for broker method array
template<typename O, typename Oc, typename R, typename ...Args>
parallel_method<O> create_binded_method(void (*_invoker)(bufin&, bufout&, std::unique_ptr<O>&, R (O::*)(Args...)), R(Oc::*_p_meth)(Args...)) {
	return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
}

// create a method pointer for broker method array
template<typename O, typename Oc, typename R, typename ...Args>
parallel_method<O> const_create_binded_method(void (*_invoker)(bufin&, bufout&, std::unique_ptr<O>&, R (O::*)(Args...) const), R(Oc::*_p_meth)(Args...) const) {
	return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
}

// create a method pointer for broker method array
template<typename O, typename R, typename ...Args>
parallel_method<O> static_create_binded_method(void (*_invoker)(bufin&, bufout&, std::unique_ptr<O>&, R (*)(Args...)), R(*_p_meth)(Args...)) {
	return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
}

/// A broker is the (remote) part that contains the instantiation of the parallel object
template<class ParClass> class broker : private boost::noncopyable
{
public:
	inline void remote_call(int _nb, bufin& _ia, bufout& _oa) {
		(methods_.at(_nb))(_ia, _oa, p_obj_);
	}
private:

	/// A call to constructor
	template<typename ...Args> static ParClass* __constr(Args... args) {
		return new ParClass(args...);
	}
	template<typename ...Args> static void call_constr(bufin& _ia, bufout& _oa, std::unique_ptr<ParClass>& _p_obj)
	{
		if(_p_obj)
			throw std::runtime_error("Constructor has been called twice");
		LOG(debug) << "Call constructor";
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		_p_obj.reset(apply_tuple_constr(__constr<Args...>, tup));
		serialize_out<bufout, Args...>(_oa, tup);
	}

	/// A simple concurrent call to a method
	template<typename R, typename ...Args> static void conc(bufin& _ia, bufout& _oa, std::unique_ptr<ParClass>& _p_obj, R (ParClass::*_p_meth)(Args...))
	{
		if(!_p_obj)
			throw std::runtime_error("Constructor has not been called");
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		// TODO: maybe one day use  http://en.cppreference.com/w/cpp/utility/apply
		apply_tuple(_p_obj.get(), _p_meth, tup, _oa);
		serialize_out<bufout, Args...>(_oa, tup);
	}

	/// A simple concurrent call to a static method
	template<typename R, typename ...Args> static void static_conc(bufin& _ia, bufout& _oa, std::unique_ptr<ParClass>& _p_obj, R (*_p_meth)(Args...))
	{
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		apply_tuple_static( _p_meth, tup, _oa);
		serialize_out<bufout, Args...>(_oa, tup);
	}

	/// A simple concurrent call to a static method
	template<typename R, typename ...Args> static void const_conc(bufin& _ia, bufout& _oa, std::unique_ptr<ParClass>& _p_obj, R (ParClass::*_p_meth)(Args...) const)
	{
		if(!_p_obj)
			throw std::runtime_error("Constructor has not been called");
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		apply_tuple_const(_p_obj.get(), _p_meth, tup, _oa);
		serialize_out<bufout, Args...>(_oa, tup);
	}


private:
	// The list of parallel methods: will be generated by the parser and added at link
	static const std::vector<remote::parallel_method<ParClass>> methods_;
	std::unique_ptr<ParClass> p_obj_;
};

}
} // namespaces

#endif
