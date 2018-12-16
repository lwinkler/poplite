#include "POPObject.hpp"

/**
 * @author  clementval
 * @date	2012.08.22
 * This program tests asynchronous parallel object allocation
 */


using namespace std;
using namespace stress_test;

int main(int argc, char** argv) {
	// Init the pop system with arguments
	pop::system::instance(&argc, argv);
	try {
		{
			printf("Stress test: Starting test...\n");
			POPObject_iface o1;
			for (int i = 0; i < 10000; i++) {
				o1.increment();
			}

			int value = o1.getCounter();
			if (value == 10000) {
				printf("Stress test: test succeeded, destroying objects ...\n");
			} else {
				printf("Stress failed: test succeeded, destroying objects ...\n");
			}
		}
		{
			// TODO: For now each object creates 5 file descriptors. To be improved
			printf("Stress allocationtest started ...\n");
			printf("Number of objects created on your computer can be limited by ulimit\n");
			printf("ulimit -n\t To check how many files can be open on your computer\n");
			printf("ulimit -n XX\t To change the limit (XX must be a number)\n");
			printf("For this test ulimit can be set to 1024\n");
			assert(argc > 1);
			POPObject_iface o[atoi(argv[1])];
		}
	} catch (exception& e) {
		printf("Stress test failed: error '%s', destroying objects:\n", e.what());
		return 1;
	}
	return 0;
}
