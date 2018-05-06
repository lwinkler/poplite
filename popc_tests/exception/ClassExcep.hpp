#ifndef _CLASSEXCEP_PH
#define _CLASSEXCEP_PH

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "com/accesspoint.hpp"

// this include should always be right before the parallel class declaration
#include "ClassExcep.iface.hpp"

POP_CLASS ClassExcep {
public:
	ClassExcep();

	~ClassExcep();

	POP_SYNC void SeqSync(int i);
	POP_SYNC void ConcSync(int i);
	POP_SYNC void MutexSync(int i);

private:
	int e;
};
#endif
