#include "PopObject.h"

/**
 * @author  clementval
 * @date	2012.08.22
 * This program tests asynchronous parallel object allocation
 */

using namespace std;

int main(int argc, char** argv) {

	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	try {
		printf("Asynchronous allocation of parallel object\n");
		printf("Before creating PopObject o1\n");
		PopObject3_iface o1;
		printf("Before creating PopObject o2\n");
		PopObject3_iface o2;
		printf("Before creating PopObject o3\n");
		PopObject3_iface o3;
		printf("Before creating PopObject o4\n");
		PopObject3_iface o4;
		printf("Before creating array of PopObject\n");
		// Create useless objects to make sure destructor is
		// synchronized
		PopObject3_iface oo[100];
		printf("Before calling method 1 on PopObject o1\n");
		o1.firstMethod();
		printf("Before calling method 1 on PopObject o2\n");
		o2.firstMethod();
		printf("Before calling method 1 on PopObject o3\n");
		o3.firstMethod();
		printf("Before calling method 2 on PopObject o1\n");
		o1.secondMethod();
		printf("Before calling method 2 on PopObject o2\n");
		o2.secondMethod();
		printf("Before calling method 2 on PopObject o3\n");
		o3.secondMethod();
		printf("Before calling method 2 on PopObject o4\n");
		o4.secondMethod();
		printf("Method with void parameter: test succeeded, destroying objects ...\n");
	} catch (std::exception& e) {
		cout << "exception: " << e.what() << endl;
		return 1;
	}
	return 0;
}
