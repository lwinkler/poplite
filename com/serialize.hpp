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
			static void ser_el_out(Archive& ar, TT& el1){}
	};

template<typename TT, typename TS>
	struct ser_element<TT, TS&>
	{
		template<class Archive>
			static void ser_el_out(Archive& ar, TT& el1){ar & el1;}
	};

template<typename TT, typename TS>
	struct ser_element<TT, const TS&>
	{
		template<class Archive>
			static void ser_el_out(Archive& /*ar*/, TT& /*el1*/){}
	};


template<uint N>
	struct SerializeOut
	{
		template<class Archive, typename TupleS, typename TupleT>
			static void serialize_out(Archive & ar, TupleT & t1)
			{
				ser_element<typename std::tuple_element<N-1, TupleT>::type, 
				            typename std::tuple_element<N-1, TupleS>::type>::ser_el_out(ar, std::get<N-1>(t1));
				SerializeOut<N-1>::template serialize_out<Archive, TupleS, TupleT>(ar, t1);
			}
	};

template<>
	struct SerializeOut<0>
	{
		template<class Archive, typename TupleS, typename TupleT>
			static void serialize_out(Archive & /*ar*/, TupleT & /*t1*/)
			{
			}
	};

template<class Archive, typename... Args>
	void serialize_out(Archive & ar, std::tuple<Args...> & t1)
	{
		SerializeOut<sizeof...(Args)>::template serialize_out<bufin, std::tuple<Args&...>, std::tuple<Args...> >(ar, t1);
	}


template<class Archive, typename... Args>
	void serialize_out(Archive & ar, std::tuple<typename std::decay<Args>::type...> & t1)
	{
		SerializeOut<sizeof...(Args)>::template serialize_out<Archive, std::tuple<Args&...>, std::tuple<typename std::decay<Args>::type...> >(ar, t1);
	}
}


#endif
