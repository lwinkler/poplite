#ifndef POPOBJECT_PH
#define POPOBJECT_PH

/**
 * @author clementval
 * @date 2012.09.04
 * This program tests
 */

#include <vector>

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"

#include "parser/defs.hpp"

// this include should always be right before the parallel class declaration
#include "POPObject.iface.hpp"

POP_CLASS POPObject {

public:
	// POP_ALLOCATION(pop::manual_allocator())
	POPObject() {
		std::cout << "POPCobject created (by JobMgr) on machine: " << pop::system::instance().host_name() << std::endl;
	}
	~POPObject() {
		std::cout << "POPCobject: on machine: " << pop::system::instance().host_name() << " is being destroyed" << std::endl;
	}

	POP_SYNC void displayArray(std::vector<char>& _arr) {
		_arr[2] = 'b';
		_arr[5] = 'c';
		_arr[10495910] = 'z';
		_arr[60495998] = 'w';
	}
};


#endif /*POPOBJECT_PH_*/
