//
// parse_broker.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/// This file is used in the parser. It creates the code of the object broker

#ifndef _POP_PARSE_BROKER_HPP
#define _POP_PARSE_BROKER_HPP

// Create a vector of method pointers for broker

#define POP_METH(_calli_, _callb_, _ret_, _meth_, ...) POP_METH_(_calli_, _callb_, _ret_, _meth_, ARGS(__VA_ARGS__))
#define POP_METH_(_calli_, _callb_, _ret_, _meth_, ...) std::bind(&remote::broker<_parclass_>::_callb_<_ret_ APPLYC(STRIP1, __VA_ARGS__)>, \
	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &_parclass_::_meth_),

// Constructor 
#define POP_CONSTR(_alloc_, ...) POP_CONSTR_(_alloc_, ARGS(__VA_ARGS__))
#define POP_CONSTR_(_alloc_, ...) std::bind(&remote::broker<_parclass_>::call_constr<APPLY(STRIP1, __VA_ARGS__)>, \
	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),

#endif
