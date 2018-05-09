#include "method.hpp"
#include <vector>

using namespace std;

TestMethod::TestMethod() {
	// Nothing to do here
}

size_t TestMethod::TestString1(std::string value) {
	cout << value << endl;
	return value.size();
}

size_t TestMethod::TestString2(std::string& value) {
	cout << value << endl;
	return value.size();
}

size_t TestMethod::TestString3(const std::string& value) {
	cout << value << endl;
	return value.size();
}

size_t TestMethod::TestVector1(std::vector<int> value) {
	cout << value[2] << endl;
	return value[2] + value.size();
}

size_t TestMethod::TestVector2(std::vector<int>& value) {
	cout << value[2] << endl;
	return value[2] + value.size();
}

size_t TestMethod::TestVector3(const std::vector<int>& value) {
	cout << value[2] << endl;
	return value[2] + value.size();
}
