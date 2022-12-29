#include <cassert>
#include <iostream>

unsigned long long factorial(unsigned long long);

int main() {
	for (unsigned long long i = 0; i <= 20; ++i) {
		std::cout << i << ' ' << factorial(i) << '\n';
	}
}
