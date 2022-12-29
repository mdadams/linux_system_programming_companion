#include <format>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

std::string quoted_string(const std::string& s)
{
	std::stringstream ss;
	ss << std::quoted(s);
	return ss ? ss.str() : "";
}

int main(int argc, char** argv)
{
	for (int i = 0; environ[i]; ++i) {
		std::cout << std::format("{}\n", environ[i]);
	}
	for (int i = 0; i <= argc; ++i) {
		std::string a = argv[i] ? quoted_string(argv[i]) : std::string("null");
		std::cout << std::format("argv[{}] = {}\n", i, a);
	}
}
