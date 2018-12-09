
#ifndef _POPLITE_PARSER_DEFS_
#define _POPLITE_PARSER_DEFS_

/// This file contains a list of key words that are used in the parser

#ifdef _POP_PARSER_
// Only used to parse the sources and generate files for interface and broker
#define POP_CLASS class __attribute__((annotate("pop_parallel:sync")))
#define POP_CLASS_ASYNC class __attribute__((annotate("pop_parallel:async")))
#define POP_INVOKER(x) __attribute__((annotate("pop_invoker:"#x)))
#define POP_SYNC     __attribute__((annotate("pop_invoker:sync")))
#define POP_ASYNC    __attribute__((annotate("pop_invoker:async")))
#define POP_ALLOCATION(x) __attribute__((annotate("pop_allocation:"#x)))

#define POP_TEMPLATE_TYPES(...) __attribute__((annotate("pop_template_types:"#__VA_ARGS__)))\

/* For clang version older than 6, use this
#define POP_TEMPLATE_METHOD(name, invoker, types)\
__attribute__((annotate("pop_invoker:"#invoker)))\
__attribute__((annotate("pop_template_method:"#types)))\
static int template_types_of_##name;
*/

#else
// Do not use in normal time
#define POP_CLASS class
#define POP_CLASS_ASYNC class
#define POP_INVOKER(x)
#define POP_SYNC
#define POP_ASYNC
#define POP_ALLOCATION(x)
#define POP_TEMPLATE_TYPES(...)
// #define POP_TEMPLATE_METHOD(x,y,z)

#endif
#endif
