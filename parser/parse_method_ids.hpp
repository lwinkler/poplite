//
// parse_method_ids.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/// This file is used in the parser. It creates the list of method ids

#ifndef _POP_PARSE_MIDS_HPP
#define _POP_PARSE_MIDS_HPP

// Methods for interface
#define POP_METH(_calli_, _callb_, _ret_, _meth_, ...) CAT(_meth_, __COUNTER__) ,

// Constructor 
#define POP_CONSTR(_alloc_, ...) CAT(_parclass_, __COUNTER__) ,

#endif
