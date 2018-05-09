#ifndef POPOBJECT_PH_
#define POPOBJECT_PH_

/**
 * @author clementval
 * @date 2012.07.11
 */

#include <atomic>

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "com/accesspoint.hpp"

// this include should always be right before the parallel class declaration
#include "POPObject2.iface.hpp"

POP_CLASS POPObject2 {

public:
	POPObject2();
	~POPObject2();
	POP_SYNC int getCounter();
	POP_ASYNC void increment();
private:
	std::atomic<int> counter;

};


#endif /*POPOBJECT_PH_*/
