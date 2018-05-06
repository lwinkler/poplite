#ifndef _TITI_PH
#define _TITI_PH
#include "toto.hpp"

// this include should always be right before the parallel class declaration
#include "Titi.iface.hpp"
POP_CLASS Titi {


public:
	Titi();
	~Titi();

	void SetIdent(int i);
	void ComputeIdent(const pop::accesspoint& t);

private:
	int ident;
};
#endif
