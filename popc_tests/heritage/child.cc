#include <stdio.h>
#include "child.hpp"

Child::Child() {
	printf("Create Child\n");
	val = -1;
}

void Child::Method1() {
	printf("Heritage: Executing Method1 in Child no:%d\n", val);
}

void Child::callMethod(Mother_iface& c, int v) {
	c.Method1();
	c.SetVal(v);
	c.Method1();
	printf("Heritage: In Child, GetVal=%d\n", c.GetVal());
}

