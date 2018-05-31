#ifndef _TOTO_PH
#define _TOTO_PH

#ifndef _POP_PARSER_ // this accelerates parsing
#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"
#endif

#include "parser/defs.hpp"
#include "com/accesspoint.hpp"

// this include should always be right before the parallel class declaration
class Toto_iface;
#include "Toto.iface.hpp"

POP_CLASS Toto {

public:
	Toto();
	~Toto();

	void SetIdent(int i);
	int GetIdent();
	void SetContact(const pop::accesspoint& _contact) {
		contact = _contact;
	}

private:
	int ident;
	pop::accesspoint contact;
};

#endif
