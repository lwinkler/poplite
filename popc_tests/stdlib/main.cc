#include "method.hpp"

using namespace std;

int main(int _argc, char** _argv) {
	// Init the pop system with arguments
	pop::system::instance(&_argc, _argv);
	printf("cpp11: Starting test..\n");

	TestMethod_iface obj;

	std::string test_string("Hello World!");
	std::vector<int> test_vector(9);

	test_vector[2] = 42;

	auto a = obj.TestString1(test_string);

	if (a != test_string.size()) {
		cout << "Test failed, invalid return value: " << a << " should be " << test_string.size() << endl;
		return 1;
	}

	auto b = obj.TestString2(test_string);

	if (b != test_string.size()) {
		cout << "Test failed, invalid return value: " << b << " should be " << test_string.size() << endl;
		return 1;
	}

	auto c = obj.TestString3(test_string);

	if (c != test_string.size()) {
		cout << "Test failed, invalid return value: " << c << " should be " << test_string.size() << endl;
		return 1;
	}

	auto d = obj.TestVector1(test_vector);

	if (d != 42 + test_vector.size()) {
		cout << "Test failed, invalid return value: " << d << " should be " << 42 + test_vector.size() << endl;
		return 1;
	}

	auto e = obj.TestVector2(test_vector);

	if (e != 42 + test_vector.size()) {
		cout << "Test failed, invalid return value: " << e << " should be " << 42 + test_vector.size() << endl;
		return 1;
	}

	auto f = obj.TestVector3(test_vector);

	if (f != 42 + test_vector.size()) {
		cout << "Test failed, invalid return value: " << f << " should be " << 42 + test_vector.size() << endl;
		return 1;
	}

	printf("stdlib: test succeeded, destroying objects..\n");

	return 0;
}
