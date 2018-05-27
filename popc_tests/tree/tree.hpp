#ifndef TREE_PH_
#define TREE_PH_


#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"

// this include should always be right before the parallel class declaration
class Tree_iface;
#include "Tree.iface.hpp"

#define MAX_W 100

POP_CLASS Tree {
public:

	Tree();
	Tree(int id);
	~Tree();

	POP_SYNC void Create(int w, int d, int startid);


private:
	int myid;
	Tree_iface *test[MAX_W];
	int n;

};

#endif
