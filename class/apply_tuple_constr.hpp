//
// interface_combox.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// based on: http://stackoverflow.com/questions/687490/how-do-i-expand-a-tuple-into-variadic-template-functions-arguments
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef POPLITE_APPLY_TUPLE_CONSTR_H
#define POPLITE_APPLY_TUPLE_CONSTR_H

namespace pop {
/**
 * Object Function Tuple Argument Unpacking
 *
 * This recursive template unpacks the tuple parameters into
 * variadic template arguments until we reach the count of 0 where the function
 * is called with the correct parameters
 *
 * @tparam N Number of tuple arguments to unroll
 *
 * @ingroup g_util_tuple
 */
template < uint N >
struct apply_func_constr {
	template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
	static T* apply_tuple_constr(
	    T* (*f)( ArgsF... ),
	    std::tuple<ArgsT...>& t,
	    Args&... args ) {
		return apply_func_constr<N-1>::apply_tuple_constr(f, t, std::get<N-1>( t ), args... );
	}
};

//-----------------------------------------------------------------------------

/**
 * Object Function Tuple Argument Unpacking End Point
 *
 * This recursive template unpacks the tuple parameters into
 * variadic template arguments until we reach the count of 0 where the function
 * is called with the correct parameters
 *
 * @ingroup g_util_tuple
 */
template <>struct apply_func_constr<0> {
	template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
	static T* apply_tuple_constr(
	    T* (*f)( ArgsF... ),
	    std::tuple<ArgsT...>& /* t */,
	    Args&... args ) {
		return (*f)( args... );
	}
};

//-----------------------------------------------------------------------------

/**
 * Object Function Call Forwarding Using Tuple Pack Parameters
 */
// Actual apply function
template < typename T, typename... ArgsF, typename... ArgsT > T* apply_tuple_constr(T* (*f)( ArgsF... ), std::tuple<ArgsT...> & t ) {
	return apply_func_constr<sizeof...(ArgsT)>::apply_tuple_constr(f, t );
}
} // namespace
#endif
