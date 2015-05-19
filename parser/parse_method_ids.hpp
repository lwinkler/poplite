#ifndef _POP_PARSE_IFACE_HPP
#define _POP_PARSE_IFACE_HPP

// Methods for interface
#define POP_METH(_calli_, _callb_, _ret_, _meth_, ...) CAT(_meth_, __COUNTER__) ,

// Constructor 
#define POP_CONSTR(_alloc_, ...) CAT(_parclass_, __COUNTER__) ,

#endif
