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
template<typename TT, typename TS>
	struct ser_element
	{
		template<class Archive>
			static void ser_el_out(Archive& ar, TT& el1, TS& el2){}
	};

template<typename TT, typename TS>
	struct ser_element<TT, TS&>
	{
		template<class Archive>
			static void ser_el_out(Archive& ar, TT& el1, TS& el2){ar & el1;}
	};

template<typename TT, typename TS>
	struct ser_element<TT, const TS&>
	{
		template<class Archive>
			static void ser_el_out(Archive& ar, TT& el1, TS& el2){}
	};


template<uint N>
	struct SerializeOut
	{
		template<class Archive, typename... ArgsT, typename... ArgsS>
			static void serialize_out(Archive & ar, std::tuple<ArgsT...> & t1, std::tuple<ArgsS...> & t2)
			{
				//ser_element<typename std::tuple_element<N, typename std::remove_reference<decltype(t)>>::type, Archive>::ser_el_out(ar, std::get<N>(t));
				ser_element<typename std::tuple_element<N-1, std::tuple<ArgsT...>>::type, 
				            typename std::tuple_element<N-1, std::tuple<ArgsS...>>::type>::ser_el_out(ar, std::get<N-1>(t1), std::get<N-1>(t2));
				// ser_element<std::tuple_element<N-1, std::tuple<Args...>&>, Archive>::ser_el_out(ar, std::get<N-1>(t));
				// using std::tuple_element<N-1, decltype(t)>::type = typename mytype;
				SerializeOut<N-1>::serialize_out(ar, t1, t2);
			}
	};

template<>
	struct SerializeOut<0>
	{
		template<class Archive, typename... Args>
			static void serialize_out(Archive & ar, std::tuple<Args...> & /*t1*/, std::tuple<Args...> & /*t2*/)
			{
			}
	};

template<class Archive, typename... Args>
	void serialize_out(Archive & ar, std::tuple<Args...> & t1)
	{
		std::tuple<Args...> t2(t1);
		SerializeOut<sizeof...(Args)>::serialize_out(ar, t1, t2);
	}


template<class Archive, typename... Args>
	void serialize_out(Archive & ar, std::tuple<typename std::decay<Args>::type...> & t1)
	{
		std::tuple<Args...> t2;
		SerializeOut<sizeof...(Args)>::serialize_out(ar, t1, t2);
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
