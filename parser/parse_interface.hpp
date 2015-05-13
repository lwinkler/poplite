/* generate unique names for args */
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)

/* strip argument or name */
#define S1(x,y) x
#define S2(x,y) y
#define S3(x,y) x y
#define STRIP1(x) S1 x
#define STRIP2(x) S2 x
#define PAIR(x)   S3 x
#define ADDNAME(x) (x, CONCATENATE(_arg, __COUNTER__))

/* This counts the number of args */
#define NARGS_SEQ(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

/* This will let macros expand before concating them */
#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)

/* VaArgs also accepts empty args */
#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(X,##__VA_ARGS__, 4, 3, 2, 1, 0)
#define VARARG_IMPL2(base, count, ...) base##count(__VA_ARGS__)
#define VARARG_IMPL(base, count, ...) VARARG_IMPL2(base, count, __VA_ARGS__) 
#define VARARG(base, ...) VARARG_IMPL(base, VA_NARGS(__VA_ARGS__), __VA_ARGS__)

#define MyMacro0() Also works without arguments.
#define MyMacro2(x,y) [x...y]
#define MyMacro(...) VARARG(MyMacro, __VA_ARGS__)

/* This will call a macro on each argument passed in */
#define APPLY(macro, ...) CAT(APPLY_, VA_NARGS(__VA_ARGS__))(macro, __VA_ARGS__)
#define APPLY_0(m, AAA)
#define APPLY_1(m, x1) m(x1)
#define APPLY_2(m, x1, x2) m(x1), m(x2)
#define APPLY_3(m, x1, x2, x3) m(x1), m(x2), m(x3)
#define APPLY_4(m, x1, x2, x3, x4) m(x1), m(x2), m(x3), m(x4)
#define APPLY_5(m, x1, x2, x3, x4, x5) m(x1), m(x2), m(x3), m(x4), m(x5)
#define APPLY_6(m, x1, x2, x3, x4, x5, x6) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6)
#define APPLY_7(m, x1, x2, x3, x4, x5, x6, x7) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7)
#define APPLY_8(m, x1, x2, x3, x4, x5, x6, x7, x8) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8)


#define POP_METH(_calli_, _callb_, _ret_, _meth_, ...) POP_METH_(_calli_, _callb_, _ret_, _meth_, APPLY(ADDNAME, __VA_ARGS__))
#define POP_METH_(_calli_, _callb_, _ret_, _meth_, ...) inline void _meth_(APPLY(PAIR, __VA_ARGS__))    {call_sync<APPLY(STRIP1, __VA_ARGS__)>(APPLY(STRIP2, __VA_ARGS__));}

// Constructor 
#define POP_CONSTR(_calli_, _callb_, _alloc_, _meth_, ...) POP_CONSTR_(_calli_, _callb_, _alloc_, _meth_, APPLY(ADDNAME, __VA_ARGS__))
#define POP_CONSTR_(_calli_, _callb_, _alloc_, _meth_, ...) _meth_(APPLY(PAIR, __VA_ARGS__)):pop::interface(_alloc_){call_sync<APPLY(STRIP1, __VA_ARGS__)>(0, APPLY(STRIP2, __VA_ARGS__));}


