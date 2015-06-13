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

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "class/system.hpp"
#include "serialize_tuple.hpp"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>

namespace pop
{

// In and out 
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

}


#endif
