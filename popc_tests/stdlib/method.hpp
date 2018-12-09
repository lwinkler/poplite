#ifndef METHOD_PH
#define METHOD_PH

#include <string>
#include <vector>

#include "class/system.hpp"
#include "alloc/local.hpp"
#include "class/interface.hpp"

#include "parser/defs.hpp"

// this include should always be right before the parallel class declaration
#include "TestMethod.iface.hpp"

POP_CLASS TestMethod {
public:
	TestMethod();

	POP_SYNC size_t TestString1(std::string value);
	POP_SYNC size_t TestString2(std::string& value);
	POP_SYNC size_t TestString3(const std::string& value);

	POP_SYNC size_t TestVector1(std::vector<int> value);
	POP_SYNC size_t TestVector2(std::vector<int>& value);
	POP_SYNC size_t TestVector3(const std::vector<int>& value);

};
#endif
