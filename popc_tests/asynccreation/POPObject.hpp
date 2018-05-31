#ifndef POPOBJECT_PH_
#define POPOBJECT_PH_

/**
 * @author clementval
 * @date 2012.07.11
 */

#ifndef _POP_PARSER_ // this accelerates parsing
#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"
#endif

#include "parser/defs.hpp"

// this include should always be right before the parallel class declaration
#include "POPObject3.iface.hpp"

POP_CLASS POPObject3 {

public:
	POPObject3();
	~POPObject3();
	POP_SYNC void firstMethod();
	POP_ASYNC void secondMethod();

private:
};


#endif /*POPOBJECT_PH_*/
