//
// parse_interface.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/// This file is used in the parser. It creates the code of the object interface

#ifndef _POP_PARSE_IFACE_HPP
#define _POP_PARSE_IFACE_HPP

// header
#define POP_HEAD                         \
namespace pop{                           \
class _parclass_ : public pop::interface \
{                                        \
public:

// Methods for interface
#define POP_METH(_calli_, _callb_, _ret_, _meth_, ...) POP_METH_(_calli_, _callb_, _ret_, _meth_, ARGS(__VA_ARGS__))
#define POP_METH_(_calli_, _callb_, _ret_, _meth_, ...) inline _ret_ _meth_(APPLY(PAIR, __VA_ARGS__)){sync<_ret_ APPLYC(STRIP1, __VA_ARGS__)>(CAT(broker::_meth_,__COUNTER__) APPLYC(STRIP2, __VA_ARGS__));}

// Constructor 
#define POP_CONSTR(_alloc_, ...) POP_CONSTR_(_alloc_, ARGS(__VA_ARGS__))
#define POP_CONSTR_(_alloc_, ...) _parclass_(APPLY(PAIR, __VA_ARGS__)):pop::interface(_executable_, _alloc_){sync<APPLY(STRIP1, __VA_ARGS__)>(CAT(broker::_parclass_,__COUNTER__) APPLYC(STRIP2, __VA_ARGS__));}

// foot
#define POP_FOOT \
};               \
}

#endif
