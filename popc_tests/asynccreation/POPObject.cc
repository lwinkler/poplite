#include "POPObject.hpp"

/**
 * @author clementval
 * @date 2012.08.22
 * This program tests asynchronous parallel object allocation
 */

using namespace std;

POPObject3::POPObject3() {
	cout << "POPObject is processing its construction (5 secondes delay) on machine:" << pop::system::instance().host_name() << endl;
	sleep(5);
	cout << "POPObject created on machine:" << pop::system::instance().host_name() << endl;
}

POPObject3::~POPObject3() {
	cout << "POPObject on machine " << pop::system::instance().host_name() << " is being destroyed" << endl;
}

void POPObject3::firstMethod() {
	cout << "First method is called on " << pop::system::instance().host_name() << endl;
}

void POPObject3::secondMethod() {
	cout << "Second method is called on " << pop::system::instance().host_name() << endl;
}

