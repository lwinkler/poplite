#include "toto.hpp"
#include "titi.hpp"

Toto::Toto() {
	printf("CallBack: Object Toto on \"%s\"\n",  pop::system::instance().host_name().c_str());
	ident = 0;
}

Toto::~Toto() {
	printf("CallBack: Destroying the object Toto...\n");
}

void Toto::SetIdent(int i) {
	printf("CallBack: SetIdent(%d) on Toto...\n", i);
	ident = i;
}

int Toto::GetIdent() {
	printf("CallBack: GetIdent on Toto, ident =%d\n", ident);

	Titi_iface t;            // create an object to call me back
	this->SetIdent(222);     // test of "this"
	Toto_iface me(contact);
	t.ComputeIdent(me); // t is going to call me back
	return (ident);          // ident has changed because of the call back
}

