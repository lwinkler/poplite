//
// apply_tuple_static.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// based on: http://stackoverflow.com/questions/687490/how-do-i-expand-a-tuple-into-variadic-template-functions-arguments
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _POP_PARSER_
#ifndef POPLITE_APPLY_TUPLE_STATIC_H
#define POPLITE_APPLY_TUPLE_STATIC_H



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
struct apply_obj_func_static {
	template < typename R, typename... ArgsF, typename... ArgsT, typename... Args >
	static R apply_tuple_static(
	    R (*f)( ArgsF... ),
	    std::tuple<ArgsT...>& t,
	    Args&&... args ) {
		return apply_obj_func_static<N-1>::apply_tuple_static( f, t, std::get<N-1>( t ), args... );
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
struct apply_obj_func_static<0> {
	template < typename R, typename... ArgsF, typename... ArgsT, typename... Args >
	static R apply_tuple_static(
	    R (*f)( ArgsF... ),
	    std::tuple<ArgsT...>& /* t */,
	    Args&&... args ) {
		return (*f)( args... );
	}
};

//-----------------------------------------------------------------------------

/**
 * Object Function Call Forwarding Using Tuple Pack Parameters
 */
// Actual apply function
template < typename R, typename... ArgsF, typename... ArgsT >
void apply_tuple_static(R (*f)( ArgsF... ), std::tuple<ArgsT...> & t, pop::bufout& _oa) {
	R r = apply_obj_func_static<sizeof...(ArgsT)>::apply_tuple_static( f, t );
	_oa << r;
	// _oa << t;
}

//-----------------------------------------------------------------------------

/**
 * Object Function Call Forwarding Using Tuple Pack Parameters
 */
// Template overload for void methods
template < typename... ArgsF, typename... ArgsT >
void apply_tuple_static(void (*f)( ArgsF... ), std::tuple<ArgsT...> & t, pop::bufout& _oa) {
	apply_obj_func_static<sizeof...(ArgsT)>::apply_tuple_static( f, t );
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
struct apply_func_static {
	template < typename R, typename... ArgsF, typename... ArgsT, typename... Args >
	static R apply_tuple_static( R (*f)( ArgsF... ),
	                             std::tuple<ArgsT...>& t,
	                             Args&&... args ) {
		return apply_func_static<N-1>::apply_tuple_static( f, t, std::get<N-1>( t ), args... );
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
struct apply_func_static<0> {
	template < typename R, typename... ArgsF, typename... ArgsT, typename... Args >
	static R apply_tuple_static(R(*f)( ArgsF...),
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
R apply_tuple_static( R (*f)(ArgsF...),
                      std::tuple<ArgsT...> & t ) {
	return apply_func_static<sizeof...(ArgsT)>::apply_tuple_static( f, t );
}
} // namespace
#endif
#endif
