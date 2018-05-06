#ifndef _TOTO_PH
#define _TOTO_PH

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "com/accesspoint.hpp"

// this include should always be right before the parallel class declaration
#include "Toto.iface.hpp"

POP_CLASS Toto {

public:
	Toto();
	~Toto();

	void SetIdent(int i);
	int GetIdent();
	void SetContact(const pop::accesspoint& _contact){contact = _contact;}

private:
	int ident;
	pop::accesspoint contact;
};

#endif
