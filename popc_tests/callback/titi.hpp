#ifndef _TITI_PH
#define _TITI_PH
#include "toto.hpp"

// this include should always be right before the parallel class declaration
#include "Titi.iface.hpp"
POP_CLASS Titi {


public:
	Titi();
	~Titi();

	POP_SYNC void SetIdent(int i);
	POP_SYNC void ComputeIdent(Toto_iface& t);

private:
	int ident;
};
#endif
