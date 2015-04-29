#ifndef POPLITE_APPLY_TUPLE_H
#define POPLITE_APPLY_TUPLE_H


// based on: http://stackoverflow.com/questions/687490/how-do-i-expand-a-tuple-into-variadic-template-functions-arguments

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
			template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
				static void applyTuple( T* pObj,
						void (T::*f)( ArgsF... ),
						const std::tuple<ArgsT...>& t,
						Args... args )
				{
					apply_obj_func<N-1>::applyTuple( pObj, f, t, std::get<N-1>( t ), args... );
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
			template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
				static void applyTuple( T* pObj,
						void (T::*f)( ArgsF... ),
						const std::tuple<ArgsT...>& /* t */,
						Args... args )
				{
					(pObj->*f)( args... );
				}
		};

	//-----------------------------------------------------------------------------

	/**
	 * Object Function Call Forwarding Using Tuple Pack Parameters
	 */
	// Actual apply function
	template < typename T, typename... ArgsF, typename... ArgsT >
		void applyTuple( T* pObj,
				void (T::*f)( ArgsF... ),
				std::tuple<ArgsT...> const& t )
		{
			apply_obj_func<sizeof...(ArgsT)>::applyTuple( pObj, f, t );
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
			template < typename... ArgsF, typename... ArgsT, typename... Args >
				static void applyTuple( void (*f)( ArgsF... ),
						const std::tuple<ArgsT...>& t,
						Args... args )
				{
					apply_func<N-1>::applyTuple( f, t, std::get<N-1>( t ), args... );
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
			template < typename... ArgsF, typename... ArgsT, typename... Args >
				static void applyTuple( void (*f)( ArgsF... ),
						const std::tuple<ArgsT...>& /* t */,
						Args... args )
				{
					f( args... );
				}
		};

	//-----------------------------------------------------------------------------

	/**
	 * Static Function Call Forwarding Using Tuple Pack Parameters
	 */
	// Actual apply function
	template < typename... ArgsF, typename... ArgsT >
		void applyTuple( void (*f)(ArgsF...),
				std::tuple<ArgsT...> const& t )
		{
			apply_func<sizeof...(ArgsT)>::applyTuple( f, t );
		}
} // namespace
#endif
