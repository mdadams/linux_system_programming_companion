#include "ticketlock1.hpp"
#include "mutex_test.hpp"

int main(int argc, char** argv) {
	mutex_tester<ticketlock> tester;
	return tester.main(argc, argv);
}
