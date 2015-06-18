
#ifndef _POPLITE_PARSER_DEFS_
#define _POPLITE_PARSER_DEFS_

#define parallel __attribute__((annotate("parallel")))
#define sync     __attribute__((annotate("sync")))
#define async    __attribute__((annotate("async")))
#define allocation(x) __attribute__((annotate(#x)))

#endif
