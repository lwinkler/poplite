#include <stdio.h>
#include "tree.hpp"

// TODO: Cannot have more than 3 4 Tree sizes

int main(int argc, char** argv) {
	// Init the pop system with arguments
	pop::system::instance(&argc, argv);
	printf("Tree: Starting test..\n");

	if (argc <= 2) {
		printf("Usage: tree <width> <depth>\n");
		return 1;
	}
	int w, d;
	if (sscanf(argv[1], "%d", &w) != 1 || sscanf(argv[1], "%d", &d) != 1) {
		printf("Invalid argument\nUsage: tree <width> <depth>\n");
		return 1;
	}
	Tree_iface root;
	root.Create(w, d, 1);
	printf("Tree: test succeeded, destroying objects..\n");

	return 0;
}
