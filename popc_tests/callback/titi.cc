#include <stdio.h>
#include "titi.hpp"

Titi::Titi() {
	printf("CallBack: Object Titi created  %s\n", pop::system::instance().host_name().c_str());
	ident = -1;
}

Titi::~Titi() {
	printf("CallBack: Destroying the object Titi...\n");
}

void Titi::SetIdent(int i) {
	printf("CallBack: SetIdent() on Titi...\n");

	ident = i;
}

void Titi::ComputeIdent(const pop::accesspoint& _ap) {
	printf("CallBack: ComputeIdent() on Titi...\n");
	// note: unlike popc, we cannot serialize an interface TODO ?
	Toto_iface t(_ap);
	t.SetIdent(ident);
}

