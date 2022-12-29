#include <iostream>

bool greet() {
	return bool(std::cout << "Hello, World\n" << std::flush);
}
