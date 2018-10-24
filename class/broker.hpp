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

namespace pop {
namespace remote {
template<class ParClass> using parallel_method      = std::function<void(bufin&, bufout&, ParClass&)>;
template<class ParClass> using parallel_constructor = std::function<ParClass*(bufin&, bufout&, ParClass*&)>;

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
	template<class Archive> void save(Archive & _ar, const unsigned int _version) const {
		// invoke serialization of the base class
		LOG(debug) << "Save interface container at " << p_iface_->contact().host_name;
		assert(p_iface_ != nullptr);
		_ar << p_iface_->contact();
	}

	template<class Archive> void load(Archive & _ar, const unsigned int _version) {
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
template<class T> struct pop_decay {
	typedef typename std::decay<T>::type type;
};

template<class T> struct pop_decay<T&> {
	/// typedef pop::accesspoint type;
	typedef typename std::conditional<std::is_base_of<pop::interface, typename std::remove_reference<T>::type>::value, iface_container<T>, typename std::decay<T>::type>::type type;
	// typedef typename std::decay<T>::type type;
};

template<class Archive, typename... Args>
void serialize_out(Archive & ar, std::tuple<typename pop_decay<Args>::type...> & t1) {
	SerializeOut<sizeof...(Args)>::template serialize_out<Archive, std::tuple<Args&...>, std::tuple<typename pop_decay<Args>::type...> >(ar, t1);
}


// create a method pointer for broker method array
template<typename O, typename Oc, typename R, typename ...Args>
parallel_method<O> create_binded_method(void (*_invoker)(bufin&, bufout&, O&, R (O::*)(Args...)), R(Oc::*_p_meth)(Args...)) {
	return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
}

// create a method pointer for broker method array
template<typename O, typename Oc, typename R, typename ...Args>
parallel_method<O> const_create_binded_method(void (*_invoker)(bufin&, bufout&, O&, R (O::*)(Args...) const), R(Oc::*_p_meth)(Args...) const) {
	return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
}

// create a method pointer for broker method array
template<typename O, typename R, typename ...Args>
parallel_method<O> static_create_binded_method(void (*_invoker)(bufin&, bufout&, O&, R (*)(Args...)), R(*_p_meth)(Args...)) {
	return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
}

/// A broker is the (remote) part that contains the instantiation of the parallel object
template<class ParClass> class broker : private boost::noncopyable {
public:
	inline void remote_call(int _nb, bufin& _ia, bufout& _oa) {
		if(_nb < static_cast<int>(methods_.size())) {
			methods_.at(_nb)(_ia, _oa, *p_obj_);
		} else {
			if(p_obj_)
				throw std::runtime_error("Constructor has been called twice");
			ParClass* p_obj{};
			constr_methods_.at(_nb - methods_.size())(_ia, _oa, p_obj);
			p_obj_.reset(p_obj);
		}
	}

	template<typename ...Args> static ParClass* call_constr(bufin& _ia, bufout& _oa) {
		// TODO remove
		LOG(debug) << "Call constructor";
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		ParClass* ret = apply_tuple_constr(__constr<Args...>, tup);
		serialize_out<bufout, Args...>(_oa, tup);
		return ret;
	}

	/// A simple concurrent call to a method
	template<typename R, typename ...Args> static void conc(bufin& _ia, bufout& _oa, ParClass& _obj, R (ParClass::*_p_meth)(Args...)) {
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		// TODO: maybe one day use  http://en.cppreference.com/w/cpp/utility/apply
		apply_tuple(&_obj, _p_meth, tup, _oa);
		serialize_out<bufout, Args...>(_oa, tup);
	}

	/// A simple concurrent call to a static method
	template<typename R, typename ...Args> static void static_conc(bufin& _ia, bufout& _oa, ParClass& /*_obj*/, R (*_p_meth)(Args...)) {
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		apply_tuple_static( _p_meth, tup, _oa);
		serialize_out<bufout, Args...>(_oa, tup);
	}

	/// A simple concurrent call to a constructor method
	/*
	template<typename ...Args> static void constructor_conc(bufin& _ia, bufout& _oa, ParClass*&) {
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		apply_tuple_static(call_constr<Args...>, tup, _oa);
		serialize_out<bufout, Args...>(_oa, tup);
	}*/

	/// A simple concurrent call to a static method
	template<typename R, typename ...Args> static void const_conc(bufin& _ia, bufout& _oa, ParClass& _obj, R (ParClass::*_p_meth)(Args...) const) {
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		apply_tuple_const(_obj, _p_meth, tup, _oa);
		serialize_out<bufout, Args...>(_oa, tup);
	}

	inline std::unique_ptr<ParClass>& ptr_obj() {
		return p_obj_;
	}


private:
	/// A call to constructor
	template<typename ...Args> static ParClass* __constr(Args... args) {
		return new ParClass(args...);
	}

	// The list of parallel methods: will be generated by the parser and added at link
	static const std::vector<remote::parallel_method<ParClass>> methods_;
	static const std::vector<remote::parallel_constructor<ParClass>> constr_methods_;
	std::unique_ptr<ParClass> p_obj_;
};

// create a method pointer for broker method array
template<typename O, typename Oc, typename R, typename ...Args>
parallel_method<O> create_binded_methodAAA(void (*_invoker)(bufin&, bufout&, O&, R (O::*)(Args...)), R(Oc::*_p_meth)(Args...)) {
	return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
}

// create a constructor method for broker method array
// TODO LW: rearrange code
template<typename O, typename ...Args>
parallel_constructor<O> create_binded_constructor() {
	O* (*invoker)(bufin&, bufout&) = &remote::broker<O>::call_constr;
	return std::bind(invoker, std::placeholders::_1, std::placeholders::_2);
}

}
} // namespaces

#endif
