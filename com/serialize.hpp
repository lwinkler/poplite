//
// serialize.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POPLITE_SERIALIZE_H
#define POPLITE_SERIALIZE_H

#include "class/system.hpp"
#include "serialize_tuple.hpp"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>

namespace pop
{

template<typename T> inline void serialize_in_iface(bufout& _oa, const T& _obj){_obj >> _oa;}
template<typename T> inline void serialize_in_brok(bufin& _ia, T& _obj)        {_ia  >> _obj;}

template<typename T> inline void serialize_out_brok(bufout& _oa, T& _obj)       {if(! std::is_const<T>::value && (std::is_pointer<T>::value || std::is_reference<T>::value)) _obj >> _oa;}
template<typename T> inline void serialize_out_iface(bufin& _ia, const T& _obj) {if(! std::is_const<T>::value && (std::is_pointer<T>::value || std::is_reference<T>::value)) _ia  >> _obj;}


template<typename T>
	struct ser_element
	{
		static void ser_el_out(T& el){}
	};

template<typename T>
	struct ser_element<T&>
	{
		static void ser_el_out(T& el){/*ar & el;*/}
	};

template<typename T>
	struct ser_element<const T&>
	{
		static void ser_el_out(const T& el){}
	};


template<uint N>
	struct SerializeOut
	{
		template<class Archive, typename... Args>
			static void serialize_out(Archive & ar, std::tuple<Args...> & t)
			{
				//ser_element<typename std::tuple_element<N, typename std::remove_reference<decltype(t)>>::type, Archive>::ser_el_out(ar, std::get<N>(t));
				ser_element<typename std::tuple_element<N-1, std::tuple<Args...>>::type>::ser_el_out(std::get<N-1>(t));
				// ser_element<std::tuple_element<N-1, std::tuple<Args...>&>, Archive>::ser_el_out(ar, std::get<N-1>(t));
				// using std::tuple_element<N-1, decltype(t)>::type = typename mytype;
				SerializeOut<N-1>::serialize_out(ar, t);
			}
	};

template<>
	struct SerializeOut<0>
	{
		template<class Archive, typename... Args>
			static void serialize_out(Archive & ar, std::tuple<Args...> & t)
			{
			}
	};

template<class Archive, typename... Args>
	void serialize_out(Archive & ar, std::tuple<Args...> & t)
	{
		SerializeOut<sizeof...(Args)>::serialize_out(ar, t);
	}


/*
template<uint N>
	struct ApplySerialization
	{
		template<class Archive, typename Function, typename... Args>
			static void apply_serialization(Archive & ar, Function f, std::tuple<Args...> & t)
			{
				f(ar, std::get<N-1>(t));
				ApplySerialization<N-1>::apply_serialization(ar, f, t);
			}
	};

template<>
	struct ApplySerialization<0>
	{
		template<class Archive, typename Function, typename... Args>
			static void apply_serialization(Archive & ar, Function f, std::tuple<Args...> & t)
			{
			}
	};

template<class Archive, typename Function, typename... Args>
	void apply_serialization(Archive & ar, Function f, std::tuple<Args...> & t)
	{
		ApplySerialization<sizeof...(Args)>::apply_serialization(ar, f, t);
	}
	*/

// In and out 
#if 0
template<typename T> struct in
{
	inline in(T& val) : m_val(val){}
	inline void marshallInterface(bufout& oa){oa<<m_val;}
	inline void unmarshallBroker(){/*unmarshall(m_val);*/}

	inline void marshallBroker(bufout& oa){}
	inline void unmarshallInterface(){}

	inline const T& operator = (const T& val){m_val = val;}
	inline operator T(){return m_val;}

	private:
	T& m_val;
};

template<typename T> struct out
{
	inline out(T& val) : m_val(val){}
	inline void marshallInterface(bufout& oa){}
	inline void unmarshallBroker(){}

	inline void marshallBroker(bufout& oa){oa<<m_val;}
	inline void unmarshallInterface(){/*unmarshall(m_val);*/}

	inline const T& operator = (const T& val){m_val = val;}
	inline operator T(){return m_val;}

	private:
	T& m_val;
};

template<typename T> struct inout
{
	inline inout(T& val) : m_val(val){}
	inline void marshallInterface(bufout& oa){oa<<m_val;}
	inline void unmarshallBroker(){/*unmarshall(m_val);*/}

	inline void marshallBroker(bufout& oa){oa<<m_val;}
	inline void unmarshallInterface(){/*unmarshall(m_val);*/}

	inline const T& operator = (const T& val){m_val = val;}
	inline operator T(){return m_val;}

	private:
	T& m_val;
};
#endif

}


#endif
