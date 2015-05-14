#ifndef _POP_PARSE_BROKER_HPP
#define _POP_PARSE_BROKER_HPP

// Create a vector of method pointers for broker

#define POP_METH(_calli_, _callb_, _ret_, _meth_, ...) POP_METH_(_calli_, _callb_, _ret_, _meth_, APPLY(ADDNAME, __VA_ARGS__))
#define POP_METH_(_calli_, _callb_, _ret_, _meth_, ...) std::bind(&remote::broker<_parclass_>::_callb_<_ret_ APPLYC(STRIP1, __VA_ARGS__)>, \
	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &_parclass_::_meth_),

// Constructor 
#define POP_CONSTR(_calli_, _callb_, _alloc_, ...) POP_CONSTR_(_calli_, _callb_, _alloc_, APPLY(ADDNAME, __VA_ARGS__))
#define POP_CONSTR_(_calli_, _callb_, _alloc_, ...) std::bind(&remote::broker<_parclass_>::call_constr<APPLY(STRIP1, __VA_ARGS__)>, \
	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &_parclass_::__constr),

#endif
