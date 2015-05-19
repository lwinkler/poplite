#ifndef _POP_PARSE_IFACE_HPP
#define _POP_PARSE_IFACE_HPP

// Methods for interface
#define POP_METH(_calli_, _callb_, _ret_, _meth_, ...) POP_METH_(_calli_, _callb_, _ret_, _meth_, ARGS(__VA_ARGS__))
#define POP_METH_(_calli_, _callb_, _ret_, _meth_, ...) inline _ret_ _meth_(APPLY(PAIR, __VA_ARGS__)){sync<_ret_ APPLYC(STRIP1, __VA_ARGS__)>(CAT(broker::_meth_,__LINE__) APPLYC(STRIP2, __VA_ARGS__));}

// Constructor 
#define POP_CONSTR(_calli_, _callb_, _alloc_, ...) POP_CONSTR_(_calli_, _callb_, _alloc_, ARGS(__VA_ARGS__))
#define POP_CONSTR_(_calli_, _callb_, _alloc_, ...) _parclass_(APPLY(PAIR, __VA_ARGS__)):pop::interface(_executable_, _alloc_){sync<APPLY(STRIP1, __VA_ARGS__)>(CAT(broker::_parclass_,__LINE__) APPLYC(STRIP2, __VA_ARGS__));}

#endif
