#ifndef _MOTHER_PH
#define _MOTHER_PH

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"

#include "parser/defs.hpp"

// this include should always be right before the parallel class declaration
#include "Mother.iface.hpp"
class Mother_iface;

POP_CLASS Mother {

public:

	Mother();

	POP_SYNC virtual void Method1();
	POP_SYNC virtual void callMethod(Mother_iface& c, int v);
	POP_SYNC void SetVal(int v);
	POP_SYNC int GetVal();

protected:
	int val;
};

#endif
