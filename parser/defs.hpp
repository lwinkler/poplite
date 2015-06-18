
#ifndef _POPLITE_PARSER_DEFS_
#define _POPLITE_PARSER_DEFS_

/// This file contains a list of key words that are used in the parser

#ifdef _POP_PARSER_
// Only used to parse the sources and generate files for interface and broker
#define parallel __attribute__((annotate("parallel")))
// #define sync     __attribute__((annotate("sync")))
#define async    __attribute__((annotate("async")))
#define allocation(x) __attribute__((annotate(#x)))

#else
// Do not use in normal time
#define parallel
// #define sync
#define async
#define allocation(x)

#endif
#endif
