/**
 * @author  clementval
 * @date	2012.09.04
 * This program tests marshalling and un-masrhalling of big amount of data
 */

#include "POPObject.hpp"
#include <assert.h>

namespace {

using namespace std;

// 60 MiB
const int Size = 60 * 1024 * 1024;

vector<char> arr(Size);

}  // end of anonymous namespace

int main(int argc, char** argv) {
	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	try {
		cout << "Big data: Starting test..." << endl;
		std::fill(arr.begin(), arr.end(), 'a');

		POPObject_iface o;
		o.displayArray(arr);

		assert(arr[1] == 'a');
		assert(arr[2] == 'b');
		assert(arr[5] == 'c');
		assert(arr[10495910] == 'z');
		assert(arr[60495998] == 'w');

		cout << "big data succeed !" << endl;
	} catch (exception& e) {
		printf("big data test failed: error %s:\n", e.what());
		return 1;
	}
	printf("big data: test succeeded, destroying objects:\n");
	return 0;
}
