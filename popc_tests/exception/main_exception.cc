/**
 * @author  P.Kuonen
 * @date	2012.09.04
 * This program tests exception propagation in POP-C++
 */
#include <stdio.h>
#include "ClassExcep.hpp"
#include "IOExceptions.h"

int main(int argc, char** argv) {
	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	printf("\nException: Starting test..\n");
	ClassExcep_iface e1;
	printf("Exception: Object created..\n");

	std::string check;

	for (int j = 0; j < 2; j++)
		for (int i = 0; i < 3; i++) {
			try {
				switch (i) {
					case 0:
						e1.SeqSync(j);
						break;
					case 1:
						e1.ConcSync(j);
						break;
					case 2:
						e1.MutexSync(j);
						break;
					default:
						printf("Exception: No call !\n");
						break;
				}  // switch

			} catch (IOException& e) {
				printf("Exception: main has catched the IOxception '%s' (%d)!!\n", e.what(), i);
				check += "a";
			} catch (std::exception& e) {
				printf("Exception: main has catched the std::exception '%s' (%d)!!\n", e.what(), i);
				check += "b";
			} catch (int e) {
				printf("Exception: main has catched the int exception %d (%d)!!\n", e, i);
				check += "c";
			} catch (...) {
				printf("Exception: main has catched the unknown exception (%d)!!\n", i);
				check += "d";
			}
		}  // for

	printf("Exception: test succeeded, destroying objects...\n");
	// note: unlike popc, all exceptions are converted to one type
	// if (check != "aaaccc") {
	if (check != "bbbbbb") {
		printf("Error in main: exceptions were not caught correctly: %s\n", check.c_str());
		return 1;
	}
	return 0;
}
