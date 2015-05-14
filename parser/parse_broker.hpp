#ifndef _POP_PARSE_BROKER_HPP
#define _POP_PARSE_BROKER_HPP

// Methods for interface
#define POP_METH(_calli_, _callb_, _ret_, _meth_, ...) POP_METH_(_calli_, _callb_, _ret_, _meth_, APPLY(ADDNAME, __VA_ARGS__))
#define POP_METH_(_calli_, _callb_, _ret_, _meth_, ...) inline _ret_ _meth_(APPLY(PAIR, __VA_ARGS__))    {call_sync<_ret_,APPLY(STRIP1, __VA_ARGS__)>(__LINE__,APPLY(STRIP2, __VA_ARGS__));}

// Constructor 
#define POP_CONSTR(_calli_, _callb_, _alloc_, ...) POP_CONSTR_(_calli_, _callb_, _alloc_, APPLY(ADDNAME, __VA_ARGS__))
#define POP_CONSTR_(_calli_, _callb_, _alloc_, ...) _parclass_(APPLY(PAIR, __VA_ARGS__)):pop::interface(_alloc_){call_sync<APPLY(STRIP1, __VA_ARGS__)>(0, APPLY(STRIP2, __VA_ARGS__));}

#endif
