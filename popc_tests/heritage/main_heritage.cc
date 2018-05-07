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

void callMethod(Mother_iface& c, int v) {
	c.Method1();
	c.SetVal(v);
	c.Method1();
	cout << "Heritage: GetVal=" << c.GetVal() << endl;
}

int main(int argc, char** argv) {

	// Init the pop system with arguments
	pop::system::instance(&argc, argv);

	cout << "\nHeritage: Starting test..." << endl;

	cout << "Heritage: Creation of object m of class Mother" << endl;
	Mother_iface m;

	cout << "Heritage: Creation of object c of class Child" << endl;
	Child_iface c;

	cout << "Heritage: Call with Mother, m. SetVal to 1" << endl;
	callMethod(m, 1);

	cout << "Heritage: Call with Child, c. SetVal to 2" << endl;
	callMethod(c, 2);

	cout << "Heritage: Call callMethod of Mother with Child, c. SetVal to 3" << endl;
	m.callMethod(c, 3);

	cout << "Heritage: Call callMethod of Child with Mother, m. SetVal to 4" << endl;
	c.callMethod(m, 4);

	cout << "Heritage: Terminated, destroying objects..." << endl;

	return 0;
}
