#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "unique_fd.hpp"

int do_work() {
	unique_fd fd(open("/dev/null", O_WRONLY));
	if (!fd) {return 1;}
	std::string text("Hello, World!\n");
	if (write(fd.get(), text.data(), text.size()) != text.size()) {
		// NOTE: no need to close file descriptor here
		return 2;
	}
	// NOTE: no need to close file descriptor here
	return 0;
} // NOTE: destruction of fd will close file descriptor (if open)

int main() {return do_work();}
