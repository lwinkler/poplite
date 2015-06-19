
#ifndef _POPLITE_PARSER_DEFS_
#define _POPLITE_PARSER_DEFS_

/// This file contains a list of key words that are used in the parser

#ifdef _POP_PARSER_
// Only used to parse the sources and generate files for interface and broker
#define pop_parallel __attribute__((annotate("parallel")))
#define pop_sync     __attribute__((annotate("sync")))
#define pop_async    __attribute__((annotate("async")))
#define pop_allocation(x) __attribute__((annotate(#x)))

#else
// Do not use in normal time
#define pop_parallel
#define pop_sync
#define pop_async
#define pop_allocation(x)

#endif
#endif
