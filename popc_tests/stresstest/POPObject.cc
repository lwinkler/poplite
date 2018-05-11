#include "POPObject.hpp"
#include <unistd.h>

/**
 * @author clementval
 * @date 2012.08.22
 * This program tests asynchronous parallel object allocation
 */


using namespace std;

POPObject2::POPObject2() {
	counter = 0;
	// cout << "POPObject created (by JobMgr) on machine:" << pop::system::instance().host_name() << endl;
}

POPObject2::~POPObject2() {
	// cout << "POPObject on machine " << pop::system::instance().host_name() << " is being destroyed" << endl;
}

int POPObject2::getCounter() {
	return counter;
}

void POPObject2::increment() {
	counter++;
}

