#include "ClassExcep.hpp"
#include "IOExceptions.h"

ClassExcep::ClassExcep() {
	IOException a("My Exception Constructor", "ICI");
	e = -1;
}

ClassExcep::~ClassExcep() {
	printf("Exception: Destroying the ClassExcep object...\n");
}

void ClassExcep::SeqSync(int i) {
	IOException a("Object: My Exception SeqSync", "ICI");
	if (i == 0) {
		throw a;
	} else {
		e = 1;
		throw e;
	}
}

void ClassExcep::ConcSync(int i) {
	IOException b("Object: My Exception ConcSync", "ICI");
	if (i == 0) {
		throw b;
	} else {
		e = 2;
		throw e;
	}
}

void ClassExcep::MutexSync(int i) {
	IOException b("Object: My Exception MutexSync", "ICI");
	if (i == 0) {
		throw b;
	} else {
		e = 3;
		throw e;
	}
}

