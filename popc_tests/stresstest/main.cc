#include "POPObject.hpp"

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
		printf("Stress test: Starting test...\n");
		POPObject2_iface o1;
		for (int i = 0; i < 10000; i++) {
			o1.increment();
		}

		int value = o1.getCounter();
		if (value == 10000) {
			printf("Stress test: test succeeded, destroying objects ...\n");
		} else {
			printf("Stress failed: test succeeded, destroying objects ...\n");
		}
	} catch (exception& e) {
		printf("Stress test failed: error '%s', destroying objects:\n", e.what());
	}
	return 0;
}
