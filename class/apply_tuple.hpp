#ifndef POPLITE_APPLY_TUPLE_H
#define POPLITE_APPLY_TUPLE_H


// based on: http://stackoverflow.com/questions/687490/how-do-i-expand-a-tuple-into-variadic-template-functions-arguments
// TODO: Avoid useless copy of return values

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
				static R applyTuple( T* pObj,
						R (T::*f)( ArgsF... ),
						std::tuple<ArgsT...>& t,
						Args&&... args )
				{
					return apply_obj_func<N-1>::applyTuple( pObj, f, t, std::get<N-1>( t ), args... );
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
				static R applyTuple( T* pObj,
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
		R applyTuple( T* pObj,
				R (T::*f)( ArgsF... ),
				std::tuple<ArgsT...> & t )
		{
			return apply_obj_func<sizeof...(ArgsT)>::applyTuple( pObj, f, t );
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
				static R applyTuple( R (*f)( ArgsF... ),
						std::tuple<ArgsT...>& t,
						Args&&... args )
				{
					return apply_func<N-1>::applyTuple( f, t, std::get<N-1>( t ), args... );
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
				static R applyTuple( R (*f)( ArgsF... ),
						std::tuple<ArgsT...>& /* t */,
						Args&&... args )
				{
					return f( args... );
				}
		};

	//-----------------------------------------------------------------------------

	/**
	 * Static Function Call Forwarding Using Tuple Pack Parameters
	 */
	// Actual apply function
	template < typename R, typename... ArgsF, typename... ArgsT >
		R applyTuple( R (*f)(ArgsF...),
				std::tuple<ArgsT...> & t )
		{
			return apply_func<sizeof...(ArgsT)>::applyTuple( f, t );
		}
} // namespace
#endif
