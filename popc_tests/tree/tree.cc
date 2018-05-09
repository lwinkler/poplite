#include <stdio.h>
#include "tree.hpp"

Tree::Tree() {
	myid = 0;
	n = 0;
	printf("Node\t%d\ton\t%s\n", myid, pop::system::instance().host_name().c_str());
}

Tree::Tree(int id) {
	myid = id;
	n = 0;
	printf("Node\t%d\ton\t%s\n", myid, pop::system::instance().host_name().c_str());
}

Tree::~Tree() {
	printf("Destroy node \t%d now!\n", myid);
	for (int i = 0; i < n; i++) {
		delete test[i];
	}
}

void Tree::Create(int w, int d, int startid) {
	n = 0;
	if (d <= 0 || w <= 0) {
		printf("Node\t%d:  leaf\n", myid);
	} else {
		for (int i = 0; i < w; i++) {
			test[i] = new Tree_iface(startid + i);
			n++;
			test[i]->Create(w, d - 1, startid + w * (i + 1));
		}
	}
}

