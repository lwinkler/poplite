#ifndef POPLITE_APPLY_TUPLE_CONSTR_H
#define POPLITE_APPLY_TUPLE_CONSTR_H


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
		struct apply_func_constr
		{
			template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
				static T* applyTupleConstr(
						T* (*f)( ArgsF... ),
						const std::tuple<ArgsT...>& t,
						Args... args )
				{
					return apply_func_constr<N-1>::applyTupleConstr(f, t, std::get<N-1>( t ), args... );
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
	template <>struct apply_func_constr<0>
	{
		template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
			static T* applyTupleConstr(
					T* (*f)( ArgsF... ),
					const std::tuple<ArgsT...>& /* t */,
					Args... args )
			{
				return (*f)( args... );
			}
	};

	//-----------------------------------------------------------------------------

	/**
	 * Object Function Call Forwarding Using Tuple Pack Parameters
	 */
	// Actual apply function
	template < typename T, typename... ArgsF, typename... ArgsT > T* applyTupleConstr(T* (*f)( ArgsF... ), std::tuple<ArgsT...> const& t )
	{
		return apply_func_constr<sizeof...(ArgsT)>::applyTupleConstr(f, t );
	}
} // namespace
#endif
