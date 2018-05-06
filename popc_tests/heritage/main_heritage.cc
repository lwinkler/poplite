/**
 * @author  P.Kuonen
 * @date	2012.09.04
 * This program tests using hierarchy of class as parameters
 */
#include <stdlib.h>
#include <iostream>
#include "mother.hpp"
#include "child.hpp"

using namespace std;

void callMethod(Mother_iface c, int v) {
	c.Method1();
	c.SetVal(v);
	c.Method1();
	cout << "Heritage: GetVal=" << c.GetVal() << endl;
}

int main(int argc, char** argv) {

	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	cout << "\nHeritage: Starting test..." << endl;

	cout << "Heritage: Creation of object c of class Mother" << endl;
	Mother_iface c;

	cout << "Heritage: Creation of object c1 of class Child" << endl;
	Child_iface c1;

	cout << "Heritage: Call with Mother, c" << endl;
	callMethod(c, 1);

	cout << "Heritage: Call with Child, c1" << endl;
	callMethod(c1, 2);

	cout << "Heritage: Call callMethod of Mother with Child, c1" << endl;
	c.callMethod(c1, 3);

	cout << "Heritage: Call callMethod of Child with Mother, c" << endl;
	c1.callMethod(c, 4);

	cout << "Heritage: Terminated, destroying objects..." << endl;

	return 0;
}
