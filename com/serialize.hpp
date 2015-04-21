#ifndef POPLITE_SERIALIZE_H
#define POPLITE_SERIALIZE_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "serialize_tuple.hpp"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>

namespace pop
{

#if 1
typedef boost::archive::text_iarchive bufin;
typedef boost::archive::text_oarchive bufout;
#else
typedef boost::archive::binary_iarchive bufin;
typedef boost::archive::binary_oarchive bufout;
#endif
/*

class method_id
{
	public:
		method_id(int x_id) : id(x_id)
		{}
		int id;
	private:
		friend class boost::serialization::access;
		template<class Archive>
			void serialize(Archive & ar, const unsigned int version)
			{
				ar & id;
			}
};

template <typename T> T read(bufin& ia)
{
	  T t; ia >> t; return t;
}

template <typename T> void write(T tuple, bufout& oa)
{
	t >> oa;
}
*/

/*

template <typename... Args>
std::tuple<Args...> parse(bufin& ia) 
{
	  return std::make_tuple(read<Args>(ia)...);
}

template <typename... Args>
std::tuple<Args...> parse(const std::string& str) 
{
	  std::istringstream ia(str);
	  return parse<Args...>(ia);
}
*/

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
