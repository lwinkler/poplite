#ifndef _POP_PARSE_HPP
#define _POP_PARSE_HPP

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
#define NARGS_SEQ(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11_,_12,_13,_14,_15,_16,N,...) N
#define NARGS(...) NARGS_SEQ(, ##__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/* This will let macros expand before concating them */
#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)

/* VaArgs also accepts empty args */
// #define VA_NARGS_IMPL(_1, _2, _3, _4, _5, N, ...) N
// #define VA_NARGS(...) VA_NARGS_IMPL(X,##__VA_ARGS__, 4, 3, 2, 1, 0)
// #define VARARG_IMPL2(base, count, ...) base##count(__VA_ARGS__)
// #define VARARG_IMPL(base, count, ...) VARARG_IMPL2(base, count, __VA_ARGS__) 
// #define VARARG(base, ...) VARARG_IMPL(base, VA_NARGS(__VA_ARGS__), __VA_ARGS__)
// 
// #define MyMacro0() Also works without arguments.
// #define MyMacro2(x,y) [x...y]
// #define MyMacro(...) VARARG(MyMacro, __VA_ARGS__)

/* This will call a macro on each argument passed in */
#define  APPLY(macro, ...) CAT(APPLY_, NARGS(__VA_ARGS__))(macro, __VA_ARGS__)
#define  APPLY_0(m, none) 
#define  APPLY_1(m, x1) m(x1)
#define  APPLY_2(m, x1, x2) m(x1), m(x2)
#define  APPLY_3(m, x1, x2, x3) m(x1), m(x2), m(x3)
#define  APPLY_4(m, x1, x2, x3, x4) m(x1), m(x2), m(x3), m(x4)
#define  APPLY_5(m, x1, x2, x3, x4, x5) m(x1), m(x2), m(x3), m(x4), m(x5)
#define  APPLY_6(m, x1, x2, x3, x4, x5, x6) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6)
#define  APPLY_7(m, x1, x2, x3, x4, x5, x6, x7) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7)
#define  APPLY_8(m, x1, x2, x3, x4, x5, x6, x7, x8) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8)
#define  APPLY_9(m, x1, x2, x3, x4, x5, x6, x7, x8, x9) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9)
#define APPLY_10(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10)
#define APPLY_11(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11)
#define APPLY_12(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12)
#define APPLY_13(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12), m(x13)
#define APPLY_14(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12), m(x13), m(x14)
#define APPLY_15(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12), m(x13), m(x14), m(x15)
#define APPLY_16(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12), m(x13), m(x14), m(x15), m(x16)

#define  APPLYC(macro, ...) CAT(APPLYC_, NARGS(__VA_ARGS__))(macro, __VA_ARGS__)
#define  APPLYC_0(m, none) 
#define  APPLYC_1(m, x1) , m(x1)
#define  APPLYC_2(m, x1, x2) , m(x1), m(x2)
#define  APPLYC_3(m, x1, x2, x3) , m(x1), m(x2), m(x3)
#define  APPLYC_4(m, x1, x2, x3, x4) , m(x1), m(x2), m(x3), m(x4)
#define  APPLYC_5(m, x1, x2, x3, x4, x5) , m(x1), m(x2), m(x3), m(x4), m(x5)
#define  APPLYC_6(m, x1, x2, x3, x4, x5, x6) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6)
#define  APPLYC_7(m, x1, x2, x3, x4, x5, x6, x7) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7)
#define  APPLYC_8(m, x1, x2, x3, x4, x5, x6, x7, x8) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8)
#define  APPLYC_9(m, x1, x2, x3, x4, x5, x6, x7, x8, x9) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9)
#define APPLYC_10(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10)
#define APPLYC_11(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11)
#define APPLYC_12(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12)
#define APPLYC_13(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12), m(x13)
#define APPLYC_14(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12), m(x13), m(x14)
#define APPLYC_15(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12), m(x13), m(x14), m(x15)
#define APPLYC_16(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) , m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11), m(x12), m(x13), m(x14), m(x15), m(x16)

#endif
