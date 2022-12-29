#include "spinlock2.hpp"
#include "mutex_test.hpp"

int main(int argc, char** argv) {
	mutex_tester<spinlock> tester;
	return tester.main(argc, argv);
}
