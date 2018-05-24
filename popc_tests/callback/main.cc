/**
 * @author  P.Kuonen
 * @date	2012.09.04
 * This program tests the call back functionning
 */
#include "toto.hpp"

int main(int argc, char** argv) {
	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	try {
		printf("\nCallBack: Starting test...\n");
		Toto_iface t;
		t.SetContact(t.contact());
		t.SetIdent(1234);

		if (t.GetIdent() == -1) {
			printf("CallBack: test succeeded, destroying objects ...\n");
			return 0;
		} else {
			printf("CallBack: test failed, destroying objects ...\n");
			return 1;
		}
	} catch (std::exception& e) {
		printf("CallBack: test failed, error %s, destroying objects:\n", e.what());
		return 1;
	}
	return 0;
}
