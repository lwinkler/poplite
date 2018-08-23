//
// apply_tuple_const.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// based on: http://stackoverflow.com/questions/687490/how-do-i-expand-a-tuple-into-variadic-template-functions-arguments
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _POP_PARSER_
#ifndef POPLITE_APPLY_TUPLE_CONST_H
#define POPLITE_APPLY_TUPLE_CONST_H



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
struct apply_obj_const {
	template < typename T, typename R, typename... ArgsF, typename... ArgsT, typename... Args >
	static R apply_tuple_const( T* pObj,
	                            R (T::*f)( ArgsF... ) const,
	                            std::tuple<ArgsT...>& t,
	                            Args&&... args ) {
		return apply_obj_const<N-1>::apply_tuple_const( pObj, f, t, std::get<N-1>( t ), args... );
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
template <>
struct apply_obj_const<0> {
	template < typename T, typename R, typename... ArgsF, typename... ArgsT, typename... Args >
	static R apply_tuple_const( T* pObj,
	                            R (T::*f)( ArgsF... ) const,
	                            std::tuple<ArgsT...>& /* t */,
	                            Args&&... args ) {
		return (pObj->*f)( args... );
	}
};

//-----------------------------------------------------------------------------

/**
 * Object Function Call Forwarding Using Tuple Pack Parameters
 */
// Actual apply function
template < typename T, typename R, typename... ArgsF, typename... ArgsT >
void apply_tuple_const( T* pObj, R (T::*f)( ArgsF... ) const, std::tuple<ArgsT...> & t, pop::bufout& _oa) {
	R r = apply_obj_const<sizeof...(ArgsT)>::apply_tuple_const( pObj, f, t );
	_oa << r;
	// _oa << t;
}

//-----------------------------------------------------------------------------

/**
 * Object Function Call Forwarding Using Tuple Pack Parameters
 */
// Template overload for void methods
template < typename T, typename... ArgsF, typename... ArgsT >
void apply_tuple_const( T* pObj, void (T::*f)( ArgsF... ) const, std::tuple<ArgsT...> & t, pop::bufout& _oa) {
	apply_obj_const<sizeof...(ArgsT)>::apply_tuple_const( pObj, f, t );
	// _oa << t;
}

//-----------------------------------------------------------------------------

/**
 * Static Function Tuple Argument Unpacking
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
struct apply_const {
	template < typename R, typename... ArgsF, typename... ArgsT, typename... Args >
	static R apply_tuple_const( R (*f)( ArgsF... ),
	                            std::tuple<ArgsT...>& t,
	                            Args&&... args ) {
		return apply_const<N-1>::apply_tuple_const( f, t, std::get<N-1>( t ), args... );
	}
};

//-----------------------------------------------------------------------------

/**
 * Static Function Tuple Argument Unpacking End Point
 *
 * This recursive template unpacks the tuple parameters into
 * variadic template arguments until we reach the count of 0 where the function
 * is called with the correct parameters
 *
 * @ingroup g_util_tuple
 */
template <>
struct apply_const<0> {
	template < typename R, typename... ArgsF, typename... ArgsT, typename... Args >
	static R apply_tuple_const(R(*f)( ArgsF...),
	                           std::tuple<ArgsT...>& /* t */,
	                           Args&&... args) {
		return f(args...);
	}
};

//-----------------------------------------------------------------------------

/**
 * Static Function Call Forwarding Using Tuple Pack Parameters
 */
// Actual apply function
template < typename R, typename... ArgsF, typename... ArgsT >
R apply_tuple_const( R (*f)(ArgsF...),
                     std::tuple<ArgsT...> & t ) {
	return apply_const<sizeof...(ArgsT)>::apply_tuple_const( f, t );
}
} // namespace
#endif
#endif
