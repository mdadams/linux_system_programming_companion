#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

int main() {
	std::vector<std::string> s{"ls", "-al", "/"};
	char* args[4]{&s[0][0], &s[1][0], &s[2][0], nullptr};
	char** env = environ; // environ is global variable
	if (execve("/bin/ls", &args[0], env) < 0) {
		std::cerr << "exec failed\n";
		return 1;
	}
	assert(false); // unreachable
}
