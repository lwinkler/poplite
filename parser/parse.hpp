//
// parse.hpp
// ~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/// This file is used in the parser. It contains general functions used by other parser files.

#ifndef _POP_PARSE_HPP
#define _POP_PARSE_HPP

/* strip argument or name */
#define S1(x,y) x
#define S2(x,y) y
#define S3(x,y) x y
#define STRIP1(x) S1 x
#define STRIP2(x) S2 x
#define PAIR(x)   S3 x
#define ADDNAME(x) (x, CAT(_arg, __COUNTER__))

/* This counts the number of args */
#define NARGS_SEQ(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11_,_12,_13,_14,_15,_16,N,...) N
#define NARGS(...) NARGS_SEQ(, ##__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/* This will let macros expand before concating them */
#define PRIMITIVE_CAT(x, y) x ## y
#define PRIMITIVE_CAT3(x, y, z) x ## y ## z
#define CAT(x, y) PRIMITIVE_CAT(x, y)
#define CAT3(x, y, z) PRIMITIVE_CAT3(x, y, z)

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
#define  APPLYC_0 APPLY_0
#define  APPLYC_1 , APPLY_1
#define  APPLYC_2 , APPLY_2
#define  APPLYC_3 , APPLY_3
#define  APPLYC_4 , APPLY_4
#define  APPLYC_5 , APPLY_5
#define  APPLYC_6 , APPLY_6
#define  APPLYC_7 , APPLY_7
#define  APPLYC_8 , APPLY_8
#define  APPLYC_9 , APPLY_9
#define APPLYC_10 , APPLY_10
#define APPLYC_11 , APPLY_11
#define APPLYC_12 , APPLY_12
#define APPLYC_13 , APPLY_13
#define APPLYC_14 , APPLY_14
#define APPLYC_15 , APPLY_15
#define APPLYC_16 , APPLY_16

/* Allow to return a sequence of type + arguments */
#define  ARGS(...) CAT(ARGS_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define  ARGS_0()
#define  ARGS_1(t0 )                                                                   (t0 , _arg0)
#define  ARGS_2(t0 , t1)                                                               (t0 , _arg0), (t1, _arg1)
#define  ARGS_3(t0 , t1, t2)                                                           (t0 , _arg0), (t1, _arg1), (t2, _arg2)
#define  ARGS_4(t0 , t1, t2, t3)                                                       (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3)
#define  ARGS_5(t0 , t1, t2, t3, t4)                                                   (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4)
#define  ARGS_6(t0 , t1, t2, t3, t4, t5)                                               (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5)
#define  ARGS_7(t0 , t1, t2, t3, t4, t5, t6)                                           (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6)
#define  ARGS_8(t0 , t1, t2, t3, t4, t5, t6, t7)                                       (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6), (t7, _arg7)
#define  ARGS_9(t0 , t1, t2, t3, t4, t5, t6, t7, t8)                                   (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6), (t7, _arg7), (t8, _arg8)
#define ARGS_10(t0 , t1, t2, t3, t4, t5, t6, t7, t8, t9)                               (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6), (t7, _arg7), (t8, _arg8), (t9, _arg9)
#define ARGS_11(t0 , t1, t2, t3, t4, t5, t6, t7, t8, t9, t10)                          (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6), (t7, _arg7), (t8, _arg8), (t9, _arg9), (t10, _arg10)
#define ARGS_12(t0 , t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11)                     (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6), (t7, _arg7), (t8, _arg8), (t9, _arg9), (t10, _arg10), (t11, _arg11)
#define ARGS_13(t0 , t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12)                (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6), (t7, _arg7), (t8, _arg8), (t9, _arg9), (t10, _arg10), (t11, _arg11), (t12, _arg12)
#define ARGS_14(t0 , t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13)           (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6), (t7, _arg7), (t8, _arg8), (t9, _arg9), (t10, _arg10), (t11, _arg11), (t12, _arg12), (t13, _arg13)
#define ARGS_15(t0 , t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14)      (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6), (t7, _arg7), (t8, _arg8), (t9, _arg9), (t10, _arg10), (t11, _arg11), (t12, _arg12), (t13, _arg13), (t14, _arg14)
#define ARGS_16(t0 , t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15) (t0 , _arg0), (t1, _arg1), (t2, _arg2), (t3, _arg3), (t4, _arg4), (t5, _arg5), (t6, _arg6), (t7, _arg7), (t8, _arg8), (t9, _arg9), (t10, _arg10), (t11, _arg11), (t12, _arg12), (t13, _arg13), (t14, _arg14), (t15, _arg15)


#endif
