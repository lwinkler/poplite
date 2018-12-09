#ifndef POPOBJECT_PH_
#define POPOBJECT_PH_

/**
 * @author clementval
 * @date 2012.07.11
 */

#include <atomic>

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"

#include "parser/defs.hpp"

namespace stress_test {
// this include should always be right before the parallel class declaration
#include "stress_test/POPObject.iface.hpp"

POP_CLASS POPObject {

public:
	POPObject();
	~POPObject();
	POP_SYNC int getCounter();
	POP_ASYNC void increment();
private:
	std::atomic<int> counter;

};
} // namespace


#endif /*POPOBJECT_PH_*/
