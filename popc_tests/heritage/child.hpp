#ifndef _CHILD_PH
#define _CHILD_PH
#include "mother.hpp"

// this include should always be right before the parallel class declaration
#include "Child.iface.hpp"
POP_CLASS Child :
public Mother {

public:
	Child();

	virtual void Method1();
	virtual void callMethod(Mother_iface& c, int v);
};

#endif
