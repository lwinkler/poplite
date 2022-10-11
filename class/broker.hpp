//
// broker.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _POP_PARSER_
#ifndef POPLITE_BROKER_H
#define POPLITE_BROKER_H


#include <future>
#include "class/system.hpp"
#include "class/apply_tuple.hpp"
#include "class/apply_tuple_constr.hpp"
#include "class/apply_tuple_static.hpp"
#include "class/apply_tuple_const.hpp"
#include "com/serialize.hpp"
#include "class/interface.hpp"

namespace pop {
namespace remote {
template<class ParClass> using parallel_method            = std::function<void(bufin&, bufout&, ParClass&)>;
template<class ParClass> using parallel_constructor_sync  = std::function<ParClass* (bufin&, bufout&)>;
template<class ParClass> using parallel_constructor_async = std::function<std::shared_future<ParClass*>(bufin&, bufout&)>;

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
void serialize_out1(Archive & ar, std::tuple<typename pop_decay<Args>::type...> & t1) {
	SerializeOut<sizeof...(Args)>::template serialize_out<Archive, std::tuple<Args&...>, std::tuple<typename pop_decay<Args>::type...> >(ar, t1);
}

/// An object constructor for the broker
template<class ParClass> class broker_constructor_sync {
public:
	broker_constructor_sync() = default;

	broker_constructor_sync(ParClass* _p_obj) {
		obj_ptr_.reset(_p_obj);
	}

	inline void construct(method_id_t _method_id, bufin& _ia, bufout& _oa) {
		obj_ptr_.reset(constr_methods_.at(_method_id)(_ia, _oa));
	}

	inline ParClass& obj() {
		return *obj_ptr_;
	}

	template<typename ...Args> static ParClass* call_constr(bufin& _ia, bufout& _oa) {
		LOG(debug) << "Call constructor";
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		// LOG(debug) << tup;
		auto cstr = __constr<Args...>;
		ParClass* ret = apply_tuple_constr(cstr, tup);
		serialize_out1<bufout, Args...>(_oa, tup);
		return ret;
	}

	// create a constructor method for broker method array
	template<typename ...Args>
	static parallel_constructor_sync<ParClass> create_binded_constructor() {
		ParClass* (*invoker)(bufin&, bufout&) = &call_constr<Args...>;
		return std::bind(invoker, std::placeholders::_1, std::placeholders::_2);
	}


private:
	/// A call to constructor
	template<typename ...Args> static ParClass* __constr(Args... args) {
		LOG(debug) << "Call constructor of parclass";
		return new ParClass(args...);
	}

	static inline const std::vector<remote::parallel_constructor_sync<ParClass>> constr_methods_;

	std::unique_ptr<ParClass> obj_ptr_;
};


/// An object constructor for the broker for async creation
template<class ParClass> class broker_constructor_async {
public:

	~broker_constructor_async() {
		delete(future_.get());
	}

	inline void construct(method_id_t _method_id, bufin& _ia, bufout& _oa) {
		future_ = constr_methods_.at(_method_id)(_ia, _oa);
	}

	inline ParClass& obj() {
		return *future_.get();
	}

	template<typename ...Args> static std::shared_future<ParClass*> call_constr(bufin& _ia, bufout& _oa) {
		LOG(debug) << "Call constructor";
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		// LOG(debug) << tup;
		std::shared_future<ParClass*> ret = std::async(std::launch::async, [tup]() mutable {
			auto cstr = __constr<Args...>;
			return apply_tuple_constr(cstr, tup);
		});
		serialize_out1<bufout, Args...>(_oa, tup);
		return ret;
	}

	// create a constructor method for broker method array
	template<typename ...Args>
	static parallel_constructor_async<ParClass> create_binded_constructor() {
		std::shared_future<ParClass*> (*invoker)(bufin&, bufout&) = &call_constr<Args...>;
		return std::bind(invoker, std::placeholders::_1, std::placeholders::_2);
	}


private:
	/// A call to constructor
	template<typename ...Args> static ParClass* __constr(Args... args) {
		LOG(debug) << "Call constructor of parclass";
		return new ParClass(args...);
	}

	static inline const std::vector<remote::parallel_constructor_async<ParClass>> constr_methods_;

	std::shared_future<ParClass*> future_;
};


/// A broker is the (remote) part that contains the instantiation of the parallel object
template<class ParClass, class BrokConstr> class broker : private boost::noncopyable {
public:
	
	broker() {
	}

	broker(ParClass* _p_obj) : broker_constructor_(_p_obj) {
	}

	void remote_call(method_id_t _method_id, bufin& _ia, bufout& _oa) {
		if(_method_id < static_cast<method_id_t>(methods_.size())) {
			methods_.at(_method_id)(_ia, _oa, broker_constructor_.obj());
		} else {
			broker_constructor_.construct(_method_id - methods_.size(), _ia, _oa);
		}
	}

	/// A simple concurrent call to a method
	template<typename R, typename ...Args> static void conc(bufin& _ia, bufout& _oa, ParClass& _obj, R (ParClass::*_p_meth)(Args...)) {
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		// TODO: maybe one day use  http://en.cppreference.com/w/cpp/utility/apply
		apply_tuple(&_obj, _p_meth, tup, _oa);
		serialize_out1<bufout, Args...>(_oa, tup);
	}

	/// A simple concurrent call to a static method
	template<typename R, typename ...Args> static void static_conc(bufin& _ia, bufout& _oa, ParClass& /*_obj*/, R (*_p_meth)(Args...)) {
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		apply_tuple_static( _p_meth, tup, _oa);
		serialize_out1<bufout, Args...>(_oa, tup);
	}

	/// A simple concurrent call to a static method
	template<typename R, typename ...Args> static void const_conc(bufin& _ia, bufout& _oa, ParClass& _obj, R (ParClass::*_p_meth)(Args...) const) {
		std::tuple<typename pop_decay<Args>::type...> tup;
		_ia >> tup;
		apply_tuple_const(&_obj, _p_meth, tup, _oa);
		serialize_out1<bufout, Args...>(_oa, tup);
	}

	inline ParClass& obj() {
		return broker_constructor_.obj();
	}

	// create a method pointer for broker method array
	template<typename Oc, typename R, typename ...Args>
	static parallel_method<ParClass> create_binded_method(void (*_invoker)(bufin&, bufout&, ParClass&, R (ParClass::*)(Args...)), R(Oc::*_p_meth)(Args...)) {
		return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
	}

	// create a method pointer for broker method array
	template<typename Oc, typename R, typename ...Args>
	static parallel_method<ParClass> const_create_binded_method(void (*_invoker)(bufin&, bufout&, ParClass&, R (ParClass::*)(Args...) const), R(Oc::*_p_meth)(Args...) const) {
		return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
	}

	// create a method pointer for broker method array
	template<typename R, typename ...Args>
	static parallel_method<ParClass> static_create_binded_method(void (*_invoker)(bufin&, bufout&, ParClass&, R (*)(Args...)), R(*_p_meth)(Args...)) {
		return std::bind(_invoker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, _p_meth);
	}

private:
	// The list of parallel methods: will be generated by the parser and added at link
	static inline const std::vector<remote::parallel_method<ParClass>> methods_;

	// The object constructor for the broker
	BrokConstr broker_constructor_;
};

}
} // namespaces

#endif
#endif
