#include "POPObject.hpp"
#include <unistd.h>

/**
 * @author clementval
 * @date 2012.08.22
 * This program tests asynchronous parallel object allocation
 */


using namespace std;
namespace stress_test {

POPObject::POPObject() {
	counter = 0;
	// cout << "POPObject created (by JobMgr) on machine:" << pop::system::instance().host_name() << endl;
}

POPObject::~POPObject() {
	// cout << "POPObject on machine " << pop::system::instance().host_name() << " is being destroyed" << endl;
}

int POPObject::getCounter() {
	return counter;
}

void POPObject::increment() {
	counter++;
}
} // namespace
