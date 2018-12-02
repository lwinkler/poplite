#include "PopObject.h"

/**
 * @author clementval
 * @date 2012.08.22
 * This program tests asynchronous parallel object allocation
 */

using namespace std;

PopObject3::PopObject3() {
	cout << "PopObject is processing its construction (5 secondes delay) on machine:" << pop::system::instance().host_name() << endl;
	sleep(5);
	cout << "PopObject created on machine:" << pop::system::instance().host_name() << endl;
}

PopObject3::~PopObject3() {
	cout << "PopObject on machine " << pop::system::instance().host_name() << " is being destroyed" << endl;
}

void PopObject3::firstMethod() {
	cout << "First method is called on " << pop::system::instance().host_name() << endl;
}

void PopObject3::secondMethod() {
	cout << "Second method is called on " << pop::system::instance().host_name() << endl;
}

