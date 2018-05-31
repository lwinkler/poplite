#ifndef _MOTHER_PH
#define _MOTHER_PH

#ifndef _POP_PARSER_ // this accelerates parsing
#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"
#endif

#include "parser/defs.hpp"

// this include should always be right before the parallel class declaration
#include "Mother.iface.hpp"
class Mother_iface;

POP_CLASS Mother {

public:

	Mother();

	virtual void Method1();
	virtual void callMethod(Mother_iface& c, int v);
	void SetVal(int v);
	int GetVal();

protected:
	int val;
};

#endif
