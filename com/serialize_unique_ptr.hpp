//
// serialize_unique_ptr.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _POP_PARSER_
#ifndef POPLITE_SERIALIZE_UNIQUE_PTR_H
#define POPLITE_SERIALIZE_UNIQUE_PTR_H

#include <memory>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
namespace boost {
namespace serialization {

template<class Archive, class T>
inline void save(
    Archive & ar,
    const std::unique_ptr< T > &t,
    const unsigned int /*file_version*/
) {
	// only the raw pointer has to be saved
	const T * const base_pointer = t.get();
	ar & BOOST_SERIALIZATION_NVP(base_pointer);
}
template<class Archive, class T>
inline void load(
    Archive & ar,
    std::unique_ptr< T > &t,
    const unsigned int /*file_version*/
) {
	T *base_pointer;
	ar & BOOST_SERIALIZATION_NVP(base_pointer);
	t.reset(base_pointer);
}
template<class Archive, class T>
inline void serialize(
    Archive & ar,
    std::unique_ptr< T > &t,
    const unsigned int file_version
) {
	boost::serialization::split_free(ar, t, file_version);
}
} // namespace serialization
} // namespace boost

#endif
#endif
