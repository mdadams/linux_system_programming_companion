#include "mutex_test.hpp"
#include "mutex1.hpp"

int main(int argc, char** argv) {
	mutex_tester<mutex> tester;
	return tester.main(argc, argv);
}
