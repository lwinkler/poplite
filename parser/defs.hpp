
#ifndef _POPLITE_PARSER_DEFS_
#define _POPLITE_PARSER_DEFS_

/// This file contains a list of key words that are used in the parser

#ifdef _POP_PARSER_
// Only used to parse the sources and generate files for interface and broker
#define POP_CLASS class __attribute__((annotate("pop_parallel")))
#define POP_SYNC     __attribute__((annotate("pop_caller:sync")))
#define POP_ASYNC    __attribute__((annotate("pop_caller:async")))
#define POP_ALLOCATION(x) __attribute__((annotate("pop_allocation:"#x)))

#else
// Do not use in normal time
#define POP_CLASS class
#define POP_SYNC
#define POP_ASYNC
#define POP_ALLOCATION(x)

#endif
#endif
