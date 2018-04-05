//
// apply_tuple.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// based on: http://stackoverflow.com/questions/687490/how-do-i-expand-a-tuple-into-variadic-template-functions-arguments
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// TODO: Avoid useless copy of return values

#ifndef POPLITE_APPLY_TUPLE_H
#define POPLITE_APPLY_TUPLE_H



namespace pop
{
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
		struct apply_obj_func
		{
			template < typename T, typename R, typename... ArgsF, typename... ArgsT, typename... Args >
				static R apply_tuple( T* pObj,
						R (T::*f)( ArgsF... ),
						std::tuple<ArgsT...>& t,
						Args&&... args )
				{
					return apply_obj_func<N-1>::apply_tuple( pObj, f, t, std::get<N-1>( t ), args... );
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
		struct apply_obj_func<0>
		{
			template < typename T, typename R, typename... ArgsF, typename... ArgsT, typename... Args >
				static R apply_tuple( T* pObj,
						R (T::*f)( ArgsF... ),
						std::tuple<ArgsT...>& /* t */,
						Args&&... args )
				{
					return (pObj->*f)( args... );
				}
		};

	//-----------------------------------------------------------------------------

	/**
	 * Object Function Call Forwarding Using Tuple Pack Parameters
	 */
	// Actual apply function
	template < typename T, typename R, typename... ArgsF, typename... ArgsT >
		void apply_tuple( T* pObj, R (T::*f)( ArgsF... ), std::tuple<ArgsT...> & t, pop::bufout& _oa)
		{
			R r = apply_obj_func<sizeof...(ArgsT)>::apply_tuple( pObj, f, t );
			_oa << r;
			// _oa << t;
		}

	//-----------------------------------------------------------------------------

	/**
	 * Object Function Call Forwarding Using Tuple Pack Parameters
	 */
	// Template overload for void methods
	template < typename T, typename... ArgsF, typename... ArgsT >
		void apply_tuple( T* pObj, void (T::*f)( ArgsF... ), std::tuple<ArgsT...> & t, pop::bufout& _oa)
			{
				apply_obj_func<sizeof...(ArgsT)>::apply_tuple( pObj, f, t );
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
		struct apply_func
		{
			template < typename R, typename... ArgsF, typename... ArgsT, typename... Args >
				static R apply_tuple( R (*f)( ArgsF... ),
						std::tuple<ArgsT...>& t,
						Args&&... args )
				{
					return apply_func<N-1>::apply_tuple( f, t, std::get<N-1>( t ), args... );
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
		struct apply_func<0>
		{
			template < typename R, typename... ArgsF, typename... ArgsT, typename... Args >
				static R apply_tuple(R(*f)( ArgsF...),
						std::tuple<ArgsT...>& /* t */,
						Args&&... args)
				{
					return f(args...);
				}
		};

	//-----------------------------------------------------------------------------

	/**
	 * Static Function Call Forwarding Using Tuple Pack Parameters
	 */
	// Actual apply function
	template < typename R, typename... ArgsF, typename... ArgsT >
		R apply_tuple( R (*f)(ArgsF...),
				std::tuple<ArgsT...> & t )
		{
			return apply_func<sizeof...(ArgsT)>::apply_tuple( f, t );
		}
} // namespace
#endif
