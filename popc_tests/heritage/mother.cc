#include <stdio.h>
#include "child.hpp"

Mother::Mother() {
	val = 0;
}

void Mother::Method1() {
	printf("Heritage: Executing Method1 in Mother no:%d\n", val);
}

void Mother::SetVal(int v) {
	val = v;
}

int Mother::GetVal() {
	return val;
}

void Mother::callMethod(Mother_iface& c, int v) {
	c.Method1();
	c.SetVal(v);
	c.Method1();
	printf("Heritage: In Mother, GetVal=%d\n", c.GetVal());
}
