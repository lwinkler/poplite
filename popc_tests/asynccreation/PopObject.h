#ifndef POPOBJECT_PH_
#define POPOBJECT_PH_

/**
 * @author clementval
 * @date 2012.07.11
 */

#include <string>

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"

#include "parser/defs.hpp"

// this include should always be right before the parallel class declaration
#include "PopObject3.iface.hpp"

POP_CLASS_ASYNC PopObject3 {

public:
	// POP_ALLOCATION(pop::manual_allocator())
	PopObject3(const std::string& _label = "my_object", int _delay = 5);
	~PopObject3();
	POP_SYNC void firstMethod();
	POP_ASYNC void secondMethod();

private:
};


#endif /*PopOBJECT_PH_*/
